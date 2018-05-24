
#ifndef RENDERER_INCLUDED
#define RENDERER_INCLUDED

#include <boost/signals2.hpp>

class renderer_t
{
public:
	static int width();
	static int height();

	static int mouse_x();
	static int mouse_y();
	static int mouse_btn();

	static void start(int width, int height, int bg_color = 0, const char* title = "");
	static void stop();
};

extern boost::signals2::signal<void(int, int)> sig_renderer_start;
extern boost::signals2::signal<void()> sig_renderer_stop;
extern boost::signals2::signal<void()> sig_renderer_render;

#endif /* RENDERER_INCLUDED */
