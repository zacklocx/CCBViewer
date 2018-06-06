
#include <cmath>

#include <mutex>
#include <thread>
#include <exception>

#include <boost/bind.hpp>

#include <GL/freeglut.h>
#include <json/json.h>

#include "app_config.h"

#include "render_win.h"
#include "line_logger.h"
#include "scoped_thread.h"

#include "render.h"

class A
{
public:
	A() {}
};

void render(A a)
{
	LLOG("A") << "a";
}

namespace
{
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

	void on_update()
	{
		try
		{
			// LLOG("on_update");

			pos_x = render_win_t::mouse_x();
			pos_y = render_win_t::mouse_y();

			if(pos_x > 1000)
			{
				throw std::logic_error("pos_x is bigger than 1000");
			}
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

		scoped_thread_t logic_thread(std::thread([]() { while(render_win_t::ready()) on_update(); }),
			scoped_thread_t::action::detach);
	}

	void on_destroy()
	{
		LLOG("on_destroy");
	}

	void on_render()
	{
		glPushMatrix();
		glLoadIdentity();

		glColor3ub(255, 0, 0);

		glBegin(GL_TRIANGLES);

		int width = render_win_t::width();
		int height = render_win_t::height();

		glVertex2f(0, 0);
		glVertex2f(pos_x, pos_y);
		glVertex2f(width - 1, height - 1);

		glEnd();

		glPopMatrix();

		fps();
	}
}

int main(int argc, char** argv)
{
	try
	{
		render_cmd_t cmd;

		cmd.add(A{});

		render(cmd);

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
