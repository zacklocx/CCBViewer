
#include <thread>
#include <exception>
#include <initializer_list>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <json/json.h>
#include <GL/freeglut.h>

#include "AppConfig.h"

#include "renderer.h"
#include "line_logger.h"
#include "callback_timer.h"

int frames = 0;
std::chrono::time_point<std::chrono::system_clock> base_time;

float point_x = 0;
float point_y = 0;

void on_update()
{
	int mouse_x = renderer_t::mouse_x();
	int mouse_y = renderer_t::mouse_y();

	point_x = mouse_x;
	point_y = mouse_y;
}

void on_start(int width, int height)
{
	LLOG() << "on_start";
	LLOG() << width << " " << height;
}

void on_stop()
{
	LLOG() << "on_stop";
}

void on_render()
{
	if(0 == frames)
	{
		base_time = std::chrono::system_clock::now();
	}

	++frames;

	auto now = std::chrono::system_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - base_time);

	if(duration.count() >= 1000)
	{
		LLOG() << "fps: " << 1000.0f * frames / duration.count();
		frames = 0;
	}

	int width = renderer_t::width();
	int height = renderer_t::height();

	glPushMatrix();
	glLoadIdentity();

	glColor3ub(255, 0, 0);

	glBegin(GL_TRIANGLES);

	glVertex2f(0.0f, 0.0f);
	glVertex2f(point_x, point_y);
	glVertex2f(width, height);

	glEnd();

	glPopMatrix();
}

int main(int argc, char** argv)
{
	try
	{
		boost::asio::io_service service;

		int peroid_ms = 10;

		callback_timer_t timer(service, peroid_ms, [&]()
		{
			on_update();
		});

		sig_renderer_start.connect(boost::bind(on_start, _1, _2));
		sig_renderer_stop.connect(boost::bind<void>([&]() { on_stop(); timer.stop(); }));
		sig_renderer_render.connect(boost::bind(on_render));

		timer.start();

		std::thread event_thread([&]() { service.run(); });

		renderer_t::start(1334, 750);

		event_thread.join();
	}
	catch(std::exception& e)
	{
		LLOG() << "exception: " << e.what();
	}

	return 0;
}
