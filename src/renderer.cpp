
#include "renderer.h"

#include <cctype>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#endif

#include <GL/freeglut.h>

#include "imgui.h"
#include "imgui_glut.h"

boost::signals2::signal<void(int, int)> sig_renderer_start;
boost::signals2::signal<void()> sig_renderer_stop;
boost::signals2::signal<void()> sig_renderer_render;

namespace
{
	struct mouse_state_t
	{
		mouse_state_t() : x_(0), y_(0), btn_(-1) {}

		int x_, y_, btn_;
	};

	int window_width = 0;
	int window_height = 0;
	mouse_state_t mouse_state;

	void stop()
	{
		imgui_glut_shutdown();
		glutLeaveMainLoop();
	}

	void reshape(int width, int height)
	{
		window_width = width;
		window_height = height;

		glViewport(0, 0, window_width, window_height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, window_width, 0, window_height, -1, 1);

		glMatrixMode(GL_MODELVIEW);
	}

	void display()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		imgui_glut_prepare(window_width, window_height);

		sig_renderer_render();

		// ImGui::ShowTestWindow();
		ImGui::Render();

		glutSwapBuffers();
	}

	void idle()
	{
		display();
	}

	void normal_key_down(unsigned char key, int x, int y)
	{
		if(27 /* Escape */ == key)
		{
			stop();
		}
		else
		{
			ImGuiIO& io = ImGui::GetIO();

			if(isprint(key))
			{
				io.AddInputCharacter(key);
			}
			else
			{
				io.KeysDown[key] = true;
			}
		}
	}

	void normal_key_up(unsigned char key, int x, int y)
	{
		if(!isprint(key))
		{
			ImGuiIO& io = ImGui::GetIO();
			io.KeysDown[key] = false;
		}
	}

	void special_key_down(int key, int x, int y)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = true;
	}

	void special_key_up(int key, int x, int y)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = false;
	}

	void mouse_wheel(int dir, int x, int y)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(x, y);
		io.MouseWheel = dir;
	}

	void mouse_click(int btn, int state, int x, int y)
	{
		if(3 == btn || 4 == btn)
		{
			if(GLUT_DOWN == state)
			{
				mouse_wheel((3 == btn)? 1 : -1, x, y);
			}
		}
		else
		{
			bool left_btn = (GLUT_LEFT_BUTTON == btn);
			bool right_btn = (GLUT_RIGHT_BUTTON == btn);

			mouse_state.x_ = x;
			mouse_state.y_ = window_height - 1 - y;
			mouse_state.btn_ = left_btn? 0 : right_btn? 1 : -1;

			ImGuiIO& io = ImGui::GetIO();
			io.MousePos = ImVec2(x, y);

			if(left_btn && GLUT_DOWN == state)
			{
				io.MouseDown[0] = true;
			}
			else
			{
				io.MouseDown[0] = false;
			}

			if(right_btn == btn && GLUT_DOWN == state)
			{
				io.MouseDown[1] = true;
			}
			else
			{
				io.MouseDown[1] = false;
			}
		}
	}

	void mouse_move(int x, int y)
	{
		mouse_state.x_ = x;
		mouse_state.y_ = window_height - 1 - y;
		mouse_state.btn_ = -1;

		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(x, y);
	}

	void mouse_drag(int x, int y)
	{
		mouse_state.x_ = x;
		mouse_state.y_ = window_height - 1 - y;

		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(x, y);
	}

	void set_bg_color(int bg_color)
	{
		int red = bg_color >> 16;
		int green = (bg_color & 0x00FF00) >> 8;
		int blue = (bg_color & 0x0000FF);

		glClearColor(red / 255.0, green / 255.0, blue / 255.0, 1.0);
	}

	void toggle_vsync(bool on)
	{
	#ifdef __APPLE__
		GLint swap_interval = on? 1 : 0;
		CGLContextObj ctx = CGLGetCurrentContext();

		if(ctx)
		{
			CGLSetParameter(ctx, kCGLCPSwapInterval, &swap_interval);
		}
	#endif
	}
}

int renderer_t::width()
{
	return window_width;
}

int renderer_t::height()
{
	return window_height;
}

int renderer_t::mouse_x()
{
	return mouse_state.x_;
}

int renderer_t::mouse_y()
{
	return mouse_state.y_;
}

int renderer_t::mouse_btn()
{
	return mouse_state.btn_;
}

void renderer_t::start(int width, int height, int bg_color /* = 0 */, const char* title /* = "" */)
{
	int argc = 1;
	char _[] = "";
	char* argv[] = { _, 0 };

	glutInit(&argc, argv);

	int screen_width = glutGet(GLUT_SCREEN_WIDTH);
	int screen_height = glutGet(GLUT_SCREEN_HEIGHT);

	window_width = (width > 0)? width : screen_width;
	window_height = (height > 0)? height : screen_height;

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(window_width, window_height);
	glutCreateWindow(title);
	glutPositionWindow((screen_width - window_width) / 2, (screen_height - window_height) / 2);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(normal_key_down);
	glutKeyboardUpFunc(normal_key_up);
	glutSpecialFunc(special_key_down);
	glutSpecialUpFunc(special_key_up);
	glutMouseFunc(mouse_click);
	glutPassiveMotionFunc(mouse_move);
	glutMotionFunc(mouse_drag);

	imgui_glut_init();

	set_bg_color(bg_color);
	toggle_vsync(false);

	sig_renderer_start(window_width, window_height);
	glutMainLoop();
	sig_renderer_stop();
}

void renderer_t::stop()
{
	::stop();
}
