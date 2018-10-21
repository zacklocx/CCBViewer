
#include "window.h"

#include <cctype>

#include <GL/freeglut.h>

#include "imgui.h"
#include "imgui_glut.h"

#define RED(color) ((color & 0xFF0000) >> 16) / 255.0
#define GREEN(color) ((color & 0x00FF00) >> 8) / 255.0
#define BLUE(color) (color & 0x0000FF) / 255.0

boost::signals2::signal<void(int, int)> sig_win_create;
boost::signals2::signal<void(int, int)> sig_win_resize;
boost::signals2::signal<void()> sig_win_render;
boost::signals2::signal<void()> sig_win_destroy;

boost::signals2::signal<void(int, int)> sig_mouse_move;
boost::signals2::signal<void(int, int, int)> sig_mouse_up;
boost::signals2::signal<void(int, int, int)> sig_mouse_down;
boost::signals2::signal<void(int, int, int)> sig_mouse_drag;
boost::signals2::signal<void(int, int, int)> sig_mouse_wheel;

boost::signals2::signal<void(int)> sig_key_up;
boost::signals2::signal<void(int)> sig_key_down;

namespace
{
	bool is_ready_ = false;

	int mode_ = _2D;

	int width_ = 0;
	int height_ = 0;

	int mouse_x_ = 0;
	int mouse_y_ = 0;
	int mouse_btn_ = 0;

	bool is_is_key_down_[512];

	void reshape(int width, int height)
	{
		width_ = (width != 0)? width : 1;
		height_ = (height != 0)? height : 1;

		glViewport(0, 0, width_, height_);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		if(_2D == mode_)
		{
			glOrtho(0.0, width_, 0.0, height_, -1.0, 1.0);
		}
		else
		{
			gluPerspective(45.0, 1.0 * width_ / height, 0.1, 100.0);
		}

		glMatrixMode(GL_MODELVIEW);

		sig_win_resize(width_, height_);
	}

	void display()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		imgui_glut_newframe(width_, height_);

		sig_win_render();
		ImGui::Render();

		glutSwapBuffers();
	}

	void idle()
	{
		glutPostRedisplay();
	}

	void close()
	{
		is_ready_ = false;

		imgui_glut_shutdown();
		sig_win_destroy();
	}

	void mouse_move(int x, int y)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(x, y);

		mouse_x_ = x;
		mouse_y_ = height_ - 1 - y;
		mouse_btn_ = -1;

		sig_mouse_move(mouse_x_, mouse_y_);
	}

	void mouse_click(int btn, int state, int x, int y)
	{
		ImGuiIO& io = ImGui::GetIO();

		bool btn_up = GLUT_UP == state;
		bool btn_down = GLUT_DOWN == state;

		if(3 == btn || 4 == btn)
		{
			if(btn_down)
			{
				int dir = (3 == btn)? 1 : -1;

				io.MouseWheel += dir;
				sig_mouse_wheel(mouse_x_, mouse_y_, dir);
			}
		}
		else
		{
			bool left_btn = GLUT_LEFT_BUTTON == btn;
			bool right_btn = GLUT_RIGHT_BUTTON == btn;

			if(left_btn || right_btn)
			{
				mouse_btn_ = left_btn? 0 : 1;

				if(btn_up)
				{
					io.MouseDown[mouse_btn_] = false;
					sig_mouse_up(mouse_x_, mouse_y_, mouse_btn_);
				}
				else if(btn_down)
				{
					io.MouseDown[mouse_btn_] = true;
					sig_mouse_down(mouse_x_, mouse_y_, mouse_btn_);
				}
			}
		}
	}

	void mouse_drag(int x, int y)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(x, y);

		mouse_x_ = x;
		mouse_y_ = height_ - 1 - y;

		sig_mouse_drag(mouse_x_, mouse_y_, mouse_btn_);
	}

	void normal_key_down(unsigned char key, int x, int y)
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

		is_key_down_[key] = true;
		sig_key_down(key);
	}

	void normal_key_up(unsigned char key, int x, int y)
	{
		if(!isprint(key))
		{
			ImGuiIO& io = ImGui::GetIO();
			io.KeysDown[key] = false;
		}

		is_key_down_[key] = false;
		sig_key_up(key);
	}

	void special_key_down(int key, int x, int y)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = true;

		is_key_down_[key] = true;
		sig_key_down(key);
	}

	void special_key_up(int key, int x, int y)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = false;

		is_key_down_[key] = false;
		sig_key_up(key);
	}
}

bool window_t::is_ready()
{
	return is_ready_;
}

int window_t::width()
{
	return width_;
}

int window_t::height()
{
	return height_;
}

int window_t::mouse_x()
{
	return mouse_x_;
}

int window_t::mouse_y()
{
	return mouse_y_;
}

int window_t::mouse_btn()
{
	return mouse_btn_;
}

bool window_t::is_key_down(int key)
{
	return is_key_down_[key];
}

void window_t::create(int mode, int width, int height, int color)
{
	int argc = 1;
	char* argv[] = { (char*)"", 0 };

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	mode_ = (_2D == mode || _3D == mode)? mode : _2D;

	int screen_width = glutGet(GLUT_SCREEN_WIDTH);
	int screen_height = glutGet(GLUT_SCREEN_HEIGHT);

	width_ = (width > 0)? width : screen_width;
	height_ = (height > 0)? height : screen_height;

	glutInitWindowSize(width_, height_);
	glutInitWindowPosition((screen_width - width_) / 2, (screen_height - height_) / 2);

	glutCreateWindow("");

	glClearColor(RED(color), GREEN(color), BLUE(color), 1.0);
	glClearDepth(1.0);

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	imgui_glut_init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutCloseFunc(close);

	glutPassiveMotionFunc(mouse_move);
	glutMouseFunc(mouse_click);
	glutMotionFunc(mouse_drag);

	glutKeyboardFunc(normal_key_down);
	glutKeyboardUpFunc(normal_key_up);
	glutSpecialFunc(special_key_down);
	glutSpecialUpFunc(special_key_up);

	is_ready_ = true;

	sig_win_create(width_, height_);

	glutMainLoop();
}

void window_t::destroy()
{
	glutLeaveMainLoop();
}
