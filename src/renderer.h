
#ifndef RENDERER_INCLUDED
#define RENDERER_INCLUDED

#include <boost/signals2.hpp>

class renderer_t
{
public:
	static bool ready();

	static int width();
	static int height();

	static int mouse_x();
	static int mouse_y();
	static int mouse_btn();

	static void start(int width, int height);
	static void stop();

	static void set_bg_color(int bg_color);
	static void set_title(const char* title);

	static void toggle_vsync(bool on);
};

extern boost::signals2::signal<void(int, int)> sig_renderer_start;
extern boost::signals2::signal<void()> sig_renderer_stop;
extern boost::signals2::signal<void()> sig_renderer_render;

#endif /* RENDERER_INCLUDED */
