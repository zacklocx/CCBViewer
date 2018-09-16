
#include <cmath>

#include <mutex>
#include <thread>
#include <exception>

#include <boost/bind.hpp>

#include <json/json.h>

#include "json.h"
#include "util.h"
#include "thread.h"
#include "render.h"
#include "window.h"
#include "llog.h"

#include "a.h"
#include "demo.h"

#include <Box2D/Box2D.h>

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
		// try
		// {
		// 	// LLOG("on_update");

		// 	float x = window_t::mouse_x();
		// 	float y = window_t::mouse_y();

		// 	if(x > 1000)
		// 	{
		// 		throw std::logic_error("x is bigger than 1000");
		// 	}

		// 	// demo.set_x(x);
		// 	// demo.set_y(y);
		// }
		// catch(...)
		// {
		// 	std::lock_guard<std::mutex> lock(e_mutex);

		// 	e_ptr = std::current_exception();
		// 	window_t::destroy();
		// }
	}

	void on_create(int width, int height)
	{
		LLOG("on_create") << width << " " << height;

		thread_t<thread_exec_t::detach> logic_thread([]() { while(window_t::ready()) on_update(); });
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

void test_box2d()
{
	// Define the gravity vector.
	b2Vec2 gravity(0.0f, -10.0f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	b2World world(gravity);

	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = world.CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// The extents are the half-widths of the box.
	groundBox.SetAsBox(50.0f, 10.0f);

	// Add the ground fixture to the ground body.
	groundBody->CreateFixture(&groundBox, 0.0f);

	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	b2Body* body = world.CreateBody(&bodyDef);

	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 1.0f;

	// Override the default friction.
	fixtureDef.friction = 0.3f;

	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);

	// Prepare for simulation. Typically we use a time step of 1/60 of a
	// second (60Hz) and 10 iterations. This provides a high quality simulation
	// in most game scenarios.
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	// This is our little game loop.
	for (int32 i = 0; i < 60; ++i)
	{
		// Instruct the world to perform a single step of simulation.
		// It is generally best to keep the time step and iterations fixed.
		world.Step(timeStep, velocityIterations, positionIterations);

		// Now print the position and angle of the body.
		b2Vec2 position = body->GetPosition();
		float32 angle = body->GetAngle();

		printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
	}

	// When the world destructor is called, all bodies and joints are freed. This can
	// create orphaned pointers, so be careful about your world management.
}

int main(int argc, char** argv)
{
	try
	{
		// LLOG() << md5("123");

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

		sig_win_create.connect(boost::bind(on_create, _1, _2));
		sig_win_destroy.connect(boost::bind(on_destroy));
		sig_win_render.connect(boost::bind(on_render));

		window_t::create(1334, 750);

		if(e_ptr)
		{
			std::rethrow_exception(e_ptr);
		}

		// test_box2d();
	}
	catch(std::exception& e)
	{
		LLOG("exception") << e.what();
	}

	return 0;
}
