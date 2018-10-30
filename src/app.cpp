
#include <cmath>

#include <chrono>

#include <GL/gl.h>

#include "log.h"
#include "draw.h"
#include "util.h"
#include "timer.h"
#include "thread.h"
#include "render.h"
#include "window.h"
#include "texture.h"

#include "line_me.h"

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

int main(int argc, char** argv)
{
	try
	{
		boost::asio::io_service service;

		line_me_t game;

		timer_t timer(service, 10,
			[&](int, uint64_t)
			{
				if(window_t::is_ready())
				{
					game.idle();
				}
			}
		);

		timer.run();

		thread_t<JOIN> update_thread([&]() { service.run(); });

		sig_win_create.connect(
			[&](int width, int height)
			{
				LOG("on_create") << width << " " << height;

				LOG() << glGetString(GL_RENDERER);
				LOG() << glGetString(GL_VERSION);

				game.init();
			}
		);

		sig_win_resize.connect(
			[&](int width, int height)
			{
				LOG("on_resize") << width << " " << height;
				game.resize(width, height);
			}
		);

		sig_win_render.connect(
			[&]()
			{
				game.render();

				fps();
			}
		);

		sig_win_destroy.connect(
			[&]()
			{
				LOG("on_destroy");

				timer.stop();
				texture_t::clear();
			}
		);

		sig_key_down.connect(
			[&](int key)
			{
				LOG("on_key_down") << key;

				if(27 /* Escape */ == key)
				{
					window_t::destroy();
				}
			}
		);

		window_t::create(_2D, 600, 800, 0xA6A6A6);
	}
	catch(std::exception& e)
	{
		LOG("exception") << e.what();
	}

	return 0;
}
