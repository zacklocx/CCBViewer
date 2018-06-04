
#include <cmath>

#include <mutex>
#include <thread>
#include <exception>

#include <boost/bind.hpp>

#include <GL/freeglut.h>
#include <json/json.h>

#include "app_config.h"

#include "renderer.h"
#include "line_logger.h"
#include "scoped_thread.h"

namespace
{
	bool halt = false;

	float pos_x = 0.0f;
	float pos_y = 0.0f;

	std::mutex e_mutex;
	std::exception_ptr e_ptr = nullptr;

	void fps()
	{
		static int frame = 0;
		static auto base_time = std::chrono::system_clock::now();

		++frame;

		auto now = std::chrono::system_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - base_time);

		if(duration.count() >= 1000)
		{
			LLOG("fps") << floor(1000.0f * frame / duration.count());

			frame = 0;
			base_time = now;
		}
	}

	void on_start(int width, int height)
	{
		LLOG("on_start") << width << " " << height;

		renderer_t::set_bg_color(0xA6A6A6);
		renderer_t::set_title(APP_NAME);
		renderer_t::toggle_vsync(false);
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

		int width = renderer_t::width();
		int height = renderer_t::height();

		glVertex2f(0, 0);
		glVertex2f(pos_x, pos_y);
		glVertex2f(width - 1, height - 1);

		glEnd();

		glPopMatrix();

		fps();
	}

	void on_update()
	{
		try
		{
			if(renderer_t::ready())
			{
				LLOG("on_update");

				pos_x = renderer_t::mouse_x();
				pos_y = renderer_t::mouse_y();

				if(pos_x > 1000)
				{
					throw std::logic_error("pos_x is bigger than 1000");
				}
			}
		}
		catch(...)
		{
			std::lock_guard<std::mutex> lock(e_mutex);

			e_ptr = std::current_exception();
			renderer_t::stop();
		}
	}
}

int main(int argc, char** argv)
{
	try
	{
		scoped_thread_t logic_thread(std::thread([]() { while(!halt) on_update(); }));

		sig_renderer_start.connect(boost::bind(on_start, _1, _2));
		sig_renderer_stop.connect(boost::bind(on_stop));
		sig_renderer_render.connect(boost::bind(on_render));

		renderer_t::start(1334, 750);

		if(e_ptr)
		{
			std::rethrow_exception(e_ptr);
		}
	}
	catch(std::exception& e)
	{
		LLOG("exception") << e.what();
	}

	return 0;
}
