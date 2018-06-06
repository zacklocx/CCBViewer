
#ifndef RENDER_WIN_INCLUDED
#define RENDER_WIN_INCLUDED

#include <boost/signals2.hpp>

class render_win_t
{
public:
	static bool ready();

	static int width();
	static int height();

	static int mouse_x();
	static int mouse_y();
	static int mouse_btn();

	static void create(int width, int height);
	static void destroy();
};

extern boost::signals2::signal<void(int, int)> sig_win_create;
extern boost::signals2::signal<void()> sig_win_destroy;
extern boost::signals2::signal<void()> sig_win_render;

#endif /* RENDER_WIN_INCLUDED */
