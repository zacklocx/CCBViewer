
#ifndef WINDOW_INCLUDED
#define WINDOW_INCLUDED

#include <boost/signals2.hpp>

class window_t
{
public:
	static bool ready();

	static int width();
	static int height();

	static bool key_up(int key);
	static bool key_down(int key);

	static int mouse_x();
	static int mouse_y();
	static int mouse_btn();
	static int mouse_dir();

	static void create(int width, int height, int color);
	static void destroy();
};

extern boost::signals2::signal<void()> sig_win_create;
extern boost::signals2::signal<void()> sig_win_render;
extern boost::signals2::signal<void()> sig_win_destroy;
extern boost::signals2::signal<void(int, int)> sig_win_resize;

extern boost::signals2::signal<void(int)> sig_key_up;
extern boost::signals2::signal<void(int)> sig_key_down;

extern boost::signals2::signal<void(int, int)> sig_mouse_move;
extern boost::signals2::signal<void(int, int, int)> sig_mouse_up;
extern boost::signals2::signal<void(int, int, int)> sig_mouse_down;
extern boost::signals2::signal<void(int, int, int)> sig_mouse_drag;
extern boost::signals2::signal<void(int, int, int)> sig_mouse_wheel;

#endif /* WINDOW_INCLUDED */
