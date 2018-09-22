
#include <cmath>

#include <mutex>
#include <thread>
#include <exception>

#include <boost/bind.hpp>

#include <GL/gl.h>

#include <json/json.h>

#include "json.h"
#include "util.h"
#include "render.h"
#include "window.h"
#include "llog.h"

#include "a.h"
#include "demo.h"

#include <chipmunk/chipmunk.h>

#define GRABBABLE_MASK_BIT (1<<31)
cpShapeFilter NOT_GRABBABLE_FILTER = {CP_NO_GROUP, ~GRABBABLE_MASK_BIT, ~GRABBABLE_MASK_BIT};

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

			float x = window_t::mouse_x();
			float y = window_t::mouse_y();

			LLOG() << x << " " << y;

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
			window_t::destroy();
		}
	}

	void on_create()
	{
		LLOG("on_create");

		LLOG() << glGetString(GL_RENDERER);
		LLOG() << glGetString(GL_VERSION);
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

void test_chipmunk2d()
{
	cpSpace *space = cpSpaceNew();
	cpSpaceSetIterations(space, 30);
	cpSpaceSetGravity(space, cpv(0, -100));
	cpSpaceSetSleepTimeThreshold(space, 0.5f);
	
	cpBody *body, *staticBody = cpSpaceGetStaticBody(space);
	cpShape *shape;
	
	// Create segments around the edge of the screen.
	shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(-320,-240), cpv(-320,240), 0.0f));
	cpShapeSetElasticity(shape, 1.0f);
	cpShapeSetFriction(shape, 1.0f);
	cpShapeSetFilter(shape, NOT_GRABBABLE_FILTER);

	shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(320,-240), cpv(320,240), 0.0f));
	cpShapeSetElasticity(shape, 1.0f);
	cpShapeSetFriction(shape, 1.0f);
	cpShapeSetFilter(shape, NOT_GRABBABLE_FILTER);

	shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(-320,-240), cpv(320,-240), 0.0f));
	cpShapeSetElasticity(shape, 1.0f);
	cpShapeSetFriction(shape, 1.0f);
	cpShapeSetFilter(shape, NOT_GRABBABLE_FILTER);
	
	shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(-320,240), cpv(320,240), 0.0f));
	cpShapeSetElasticity(shape, 1.0f);
	cpShapeSetFriction(shape, 1.0f);
	cpShapeSetFilter(shape, NOT_GRABBABLE_FILTER);
}

int main(int argc, char** argv)
{
	try
	{
		LLOG() << ttos(1537074478);
		LLOG() << stot("2018-09-16 13:07:58");

		jvalue_t val;

		val["test1"] = jnull;
		val["test2"] = jint;
		val["test3"] = juint;
		val["test4"] = jreal;
		val["test5"] = jstr;
		val["test6"] = jbool;
		val["test7"] = jarr;
		val["test8"] = jobj;
		val["test9"] = jnull;

		jvalue_t* target;

		if(jquery(val, "test1", target))
		{
			LLOG() << "test1";
			*target = "zacklocx";
		}

		if(jquery(val, "test2", target))
		{
			LLOG() << "test2";
			*target = "22";
		}

		// val["test7"][0] = jint;
		// val["test7"][3] = jstr;

		// val["test8"] = val;

		// jset(val, "test7.1", 1);
		// jset(val, "test7.2", 2);
		// jset(val, "test7.4", 4);

		jsave("bin/test.json", val);

		// if(!jload("./bin/demo.json", val))
		// {
		// 	LLOG() << "json load failed";
		// }
		// // if(!jparse("{\"value1\" : true, \"value2\" : false, \"value3\" : \"3.14\", \"value4\":\"bad\"}", val))
		// // {
		// // 	LLOG() << "malformed json";
		// // }
		// else
		// {
		// 	//LLOG() << jtob(jget(val, "value2"));

		// 	LLOG("indent false") << jdump(val);
		// 	LLOG("indent true") << jdump(val);
		// 	LLOG("value4") << jdump(jget(val, "value4"));

		// 	if(jtob(jget(val, "value2")))
		// 	{
		// 		LLOG("true");
		// 	}
		// 	else
		// 	{
		// 		LLOG("false");
		// 	}

		// 	LLOG() << jtos(val);
		// 	LLOG() << jtos(jget(val, "value1"));

		// 	LLOG() << jtoi(jget(val, "value3"));
		// 	LLOG() << jtos(jget(val, "value4"));

		// 	jset(val, "value3", 6.28f);

		// 	LLOG() << jdump(val);

		// 	jsave("./bin/demo2.json", val);
		//  }

		test_chipmunk2d();

		std::thread update_thread([]() { while(window_t::ready()) on_update(); });

		sig_win_create.connect(boost::bind(on_create));
		sig_win_destroy.connect(boost::bind(on_destroy));
		sig_win_render.connect(boost::bind(on_render));

		window_t::create(1334, 750, 0xFFFFFF);

		if(e_ptr)
		{
			std::rethrow_exception(e_ptr);
		}

		update_thread.join();
	}
	catch(std::exception& e)
	{
		LLOG("exception") << e.what();
	}

	return 0;
}
