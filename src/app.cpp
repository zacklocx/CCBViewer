
#include <cmath>

#include <mutex>
#include <thread>
#include <exception>

#include <boost/bind.hpp>

#include <GL/gl.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

//#include "util.h"
#include "timer.h"
#include "render.h"
#include "window.h"
#include "log.h"
#include "thread.h"

#include "a.h"
#include "demo.h"

namespace
{
	render_cmd_t root_cmd;

	std::mutex e_mutex;
	std::exception_ptr e_ptr = nullptr;

	demo_t demo;
	texture_t texture;

	void fps()
	{
		static int frame = 0;
		static auto base_time = std::chrono::system_clock::now();

		++frame;

		auto now = std::chrono::system_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - base_time);

		if(duration.count() >= 1000)
		{
			LOG("fps") << floor(1000.0f * frame / duration.count());

			frame = 0;
			base_time = now;
		}
	}

	void on_update()
	{
		try
		{
			LOG("on_update");

			float x = window_t::mouse_x();
			//float y = window_t::mouse_y();

		// 	// LOG() << x << " " << y;

			if(x > 1000)
			{
				throw std::logic_error("x is bigger than 1000");
			}

		// 	// demo.set_x(x);
		// 	// demo.set_y(y);
		}
		catch(...)
		{
			std::lock_guard<std::mutex> lock(e_mutex);

			e_ptr = std::current_exception();
			window_t::destroy();
		}
	}

	void on_create(int width, int height)
	{
		if(window_t::is_ready())
		{
			LOG("on_create") << width << " " << height;
		}

		LOG() << glGetString(GL_RENDERER);
		LOG() << glGetString(GL_VERSION);

		texture.load("/Users/zacklocx/tmp/tex.jpg");
	}

	void on_destroy()
	{
		LOG("on_destroy");
		texture_t::clear();
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

		float x = window_t::width() / 2, y = window_t::height() / 2;
		float w = texture.width(), h = texture.height();
		float r = 45.0f;

		texture.draw(x, y, w, h, r);

		if(window_t::is_key_down('D'))
		{
			exit(-1);
		}

		fps();
	}

	void on_resize(int w, int h)
	{
		LOG("on_resize") << w << " " << h;
	}

	void on_key_down(int key)
	{
		LOG("on_key_down") << key;

		if(27 /* Escape */ == key)
		{
			window_t::destroy();
		}
	}

	void on_mouse_wheel(int x, int y, int wheel)
	{
		LOG("on_mouse_wheel") << x << " " << y << " " << wheel;
	}
}

int main(int argc, char** argv)
{
	try
	{
		boost::asio::io_service service;

		timer_t timer(service, 10,
			[&](int period_ms, uint64_t count)
			{
				LOG("ad1") << &timer;
				LOG("count") << timer.count();

				if(timer.is_running())
				{
					LOG("running");
				}
				else
				{
					LOG("not running");
				}

				timer.stop();

				if(window_t::is_ready())
				{
					on_update();
				}
			}
		);

		LOG("ad2") << &timer;

		timer.run();

		json j;
		j["pi"] = 3.14;
		j["name"] = "zacklocx";

		// LOG() << ttos(1537074478);
		// LOG() << stot("2018-09-16 13:07:58");

		// jvalue_t val;

		// val["test1"] = jnull;
		// val["test2"] = jint;
		// val["test3"] = juint;
		// val["test4"] = jreal;
		// val["test5"] = jstr;
		// val["test6"] = jbool;
		// val["test7"] = jarr;
		// val["test8"] = jobj;
		// val["test9"] = jnull;

		// jvalue_t* target;

		// if(jquery(val, "test1", target))
		// {
		// 	LOG() << "test1";
		// 	*target = "zacklocx";
		// }

		// if(jquery(val, "test2", target))
		// {
		// 	LOG() << "test2";
		// 	*target = "22";
		// }

		// val["test7"][0] = jint;
		// val["test7"][3] = jstr;

		// val["test8"] = val;

		// jset(val, "test7.1", 1);
		// jset(val, "test7.2", 2);
		// jset(val, "test7.4", 4);

		//jsave("bin/test.json", val);

		// if(!jload("./bin/demo.json", val))
		// {
		// 	LOG() << "json load failed";
		// }
		// // if(!jparse("{\"value1\" : true, \"value2\" : false, \"value3\" : \"3.14\", \"value4\":\"bad\"}", val))
		// // {
		// // 	LOG() << "malformed json";
		// // }
		// else
		// {
		// 	//LOG() << jtob(jget(val, "value2"));

		// 	LOG("indent false") << jdump(val);
		// 	LOG("indent true") << jdump(val);
		// 	LOG("value4") << jdump(jget(val, "value4"));

		// 	if(jtob(jget(val, "value2")))
		// 	{
		// 		LOG("true");
		// 	}
		// 	else
		// 	{
		// 		LOG("false");
		// 	}

		// 	LOG() << jtos(val);
		// 	LOG() << jtos(jget(val, "value1"));

		// 	LOG() << jtoi(jget(val, "value3"));
		// 	LOG() << jtos(jget(val, "value4"));

		// 	jset(val, "value3", 6.28f);

		// 	LOG() << jdump(val);

		// 	jsave("./bin/demo2.json", val);
		//  }

		thread_t<JOIN> update_thread([&]() { service.run(); });

		sig_win_create.connect(boost::bind(on_create, _1, _2));
		sig_win_resize.connect(boost::bind(on_resize, _1, _2));
		sig_win_render.connect(boost::bind(on_render));
		sig_win_destroy.connect(boost::bind(on_destroy));

		sig_win_destroy.connect([&]() { timer.stop(); });

		sig_key_down.connect(boost::bind(on_key_down, _1));

		sig_mouse_wheel.connect(boost::bind(on_mouse_wheel, _1, _2, _3));

		window_t::create(_2D, 1334, 750, 0xA6A6A6);

		LOG("exit");

		if(e_ptr)
		{
			std::rethrow_exception(e_ptr);
		}
	}
	catch(std::exception& e)
	{
		LOG("exception") << e.what();
	}

	return 0;
}
