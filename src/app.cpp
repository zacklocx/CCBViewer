
#include <cmath>

#include <mutex>
#include <thread>
#include <exception>

#include <boost/bind.hpp>

#include <json/json.h>

#include "app_info.h"

#include "json.h"
#include "utils.h"
#include "thread.h"
#include "render.h"
#include "render_win.h"
#include "llog.h"

#include "a.h"
#include "demo.h"

namespace
{
	render_cmd_t root_cmd;

	std::mutex e_mutex;
	std::exception_ptr e_ptr = nullptr;

	demo_t demo;

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

			// demo.set_x(x);
			// demo.set_y(y);
		}
		catch(...)
		{
			std::lock_guard<std::mutex> lock(e_mutex);

			e_ptr = std::current_exception();
			render_win_t::destroy();
		}
	}

	void on_create(int width, int height, const char* title)
	{
		LLOG("on_create") << width << " " << height << " " << title;

		thread_t<thread_exec_t::detach> logic_thread([]() { while(render_win_t::ready()) on_update(); });
	}

	void on_destroy()
	{
		LLOG("on_destroy");
	}

	void on_render()
	{
		a_ns::a_t a;

		// render(1);
		// render(std::string("string"));
		// render(a);
		// render(demo);

		root_cmd.emplace_back(1);
		root_cmd.emplace_back(std::string("string"));
		root_cmd.emplace_back(a);
		root_cmd.emplace_back(demo);

		render(root_cmd);
		root_cmd.clear();

		fps();
	}
}

int main(int argc, char** argv)
{
	try
	{
		// LLOG() << md5("123");

		// Json::Value jval;

		// if(!jparse("{\"value1\" : true, \"value2\" : false, \"value3\" : 3.14, \"value4\":\"bad\"}", jval))
		// {
		// 	LLOG() << "malformed json";
		// }
		// else
		// {
		// 	if(jtob(jquery(jval, "value2")))
		// 	{
		// 		LLOG("true");
		// 	}
		// 	else
		// 	{
		// 		LLOG("false");
		// 	}

		// 	LLOG() << jtof(jquery(jval, "value3"));
		// 	LLOG() << jtos(jquery(jval, "value3"));
		// }

	// 	sig_win_create.connect(boost::bind(on_create, _1, _2, _3));
	// 	sig_win_destroy.connect(boost::bind(on_destroy));
	// 	sig_win_render.connect(boost::bind(on_render));

	// 	render_win_t::create(1334, 750, "zacklocx");

	// 	if(e_ptr)
	// 	{
	// 		std::rethrow_exception(e_ptr);
	// 	}
	}
	catch(std::exception& e)
	{
		LLOG("exception") << e.what();
	}

	return 0;
}
