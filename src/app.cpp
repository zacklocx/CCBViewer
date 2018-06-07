
#include <cmath>

#include <mutex>
#include <thread>
#include <exception>

#include <boost/bind.hpp>

#include <GL/freeglut.h>
#include <json/json.h>

#include "app_config.h"

#include "thread.h"
#include "render.h"
#include "render_win.h"
#include "line_logger.h"

namespace
{
	render_cmd_t render_cmd;

	std::mutex e_mutex;
	std::exception_ptr e_ptr = nullptr;

	class demo_t
	{
	public:
		demo_t() : x_(0.0f), y_(0.0f) {}

		void set_x(float x) { x_ = x; }
		void set_y(float y) { y_ = y; }

		float get_x() const { return x_; }
		float get_y() const { return y_; }

	private:
		float x_, y_;
	};

	demo_t demo;

	void render(const demo_t& d)
	{
		glPushMatrix();
		glLoadIdentity();

		glColor3ub(255, 0, 0);

		glBegin(GL_TRIANGLES);

		int width = render_win_t::width();
		int height = render_win_t::height();

		glVertex2f(0, 0);
		glVertex2f(d.get_x(), d.get_y());
		glVertex2f(width - 1, height - 1);

		glEnd();

		glPopMatrix();
	}

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

	void on_update()
	{
		try
		{
			// LLOG("on_update");

			float x = render_win_t::mouse_x();
			float y = render_win_t::mouse_y();

			if(x > 1000)
			{
				throw std::logic_error("x is bigger than 1000");
			}

			demo.set_x(x);
			demo.set_y(y);
		}
		catch(...)
		{
			std::lock_guard<std::mutex> lock(e_mutex);

			e_ptr = std::current_exception();
			render_win_t::destroy();
		}
	}

	void on_create(int width, int height)
	{
		LLOG("on_create") << width << " " << height;

		thread_t<thread_exec_t::detach> logic_thread([]() { while(render_win_t::ready()) on_update(); });
	}

	void on_destroy()
	{
		LLOG("on_destroy");
	}

	void on_render()
	{
		render_cmd.add(demo);
		render(render_cmd);
		render_cmd.clear();

		fps();
	}
}

int main(int argc, char** argv)
{
	try
	{
		sig_win_create.connect(boost::bind(on_create, _1, _2));
		sig_win_destroy.connect(boost::bind(on_destroy));
		sig_win_render.connect(boost::bind(on_render));

		render_win_t::create(1334, 750);

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
