
#include <cmath>
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

float point_x = 0;
float point_y = 0;

void on_update()
{
	point_x = renderer_t::mouse_x();
	point_y = renderer_t::mouse_y();
}

void on_start(int width, int height)
{
	LLOG("on_start") << width << " " << height;
}

void on_stop()
{
	LLOG("on_stop");
}

void on_render()
{
	static int frame = 0;
	++frame;

	static auto base_time = std::chrono::system_clock::now();
	auto now = std::chrono::system_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - base_time);

	if(duration.count() >= 1000)
	{
		LLOG("fps") << round(1000.0f * frame / duration.count());

		frame = 0;
		base_time = now;
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

		std::thread logic_thread([&]() { service.run(); });

		renderer_t::start(1334, 750, 0xA6A6A6);

		logic_thread.join();
	}
	catch(std::exception& e)
	{
		LLOG("exception") << e.what();
	}

	return 0;
}
