
#include <cmath>

#include <thread>
#include <exception>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <GL/freeglut.h>
#include <json/json.h>

#include "AppConfig.h"

#include "renderer.h"
#include "line_logger.h"

namespace
{
	bool halt = false;

	float pos_x = 0;
	float pos_y = 0;

	void fps()
	{
		static int frame = 0;
		static auto base_time = std::chrono::system_clock::now();

		++frame;

		auto now = std::chrono::system_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - base_time);

		if(duration.count() >= 1000)
		{
			LLOG("fps") << round(1000.0f * frame / duration.count());

			frame = 0;
			base_time = now;
		}
	}

	void on_start(int width, int height)
	{
		LLOG("on_start") << width << " " << height;
	}

	void on_stop()
	{
		LLOG("on_stop");

		halt = true;
	}

	void on_render()
	{
		glPushMatrix();
		glLoadIdentity();

		glColor3ub(255, 0, 0);

		glBegin(GL_TRIANGLES);

		glVertex2f(0.0f, 0.0f);
		glVertex2f(pos_x, pos_y);
		glVertex2f(renderer_t::width(), renderer_t::height());

		glEnd();

		glPopMatrix();

		fps();
	}

	void on_update()
	{
		pos_x = renderer_t::mouse_x();
		pos_y = renderer_t::mouse_y();
	}
}

int main(int argc, char** argv)
{
	try
	{
		std::thread logic_thread([&]() { while(!halt) on_update(); });

		sig_renderer_start.connect(boost::bind(on_start, _1, _2));
		sig_renderer_stop.connect(boost::bind<void>([&]() { on_stop(); }));
		sig_renderer_render.connect(boost::bind(on_render));

		renderer_t::start(1334, 750, 0xA6A6A6, APP_NAME);

		logic_thread.join();
	}
	catch(std::exception& e)
	{
		LLOG("exception") << e.what();
	}

	return 0;
}
