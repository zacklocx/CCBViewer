
#include "window.h"

#include <cctype>

#include <unordered_map>

#include <GL/freeglut.h>

#include "imgui.h"
#include "imgui_glut.h"

#define RED(color) ((color & 0xFF0000) >> 16) / 255.0
#define GREEN(color) ((color & 0x00FF00) >> 8) / 255.0
#define BLUE(color) (color & 0x0000FF) / 255.0

boost::signals2::signal<void(int, int)> sig_win_create;
boost::signals2::signal<void(int, int)> sig_win_resize;
boost::signals2::signal<void()> sig_win_render;
boost::signals2::signal<void()> sig_win_close;

boost::signals2::signal<void(int, int)> sig_mouse_move;
boost::signals2::signal<void(int, int, int)> sig_mouse_up;
boost::signals2::signal<void(int, int, int)> sig_mouse_down;
boost::signals2::signal<void(int, int, int)> sig_mouse_drag;
boost::signals2::signal<void(int, int, int)> sig_mouse_wheel;

boost::signals2::signal<void(int)> sig_key_up;
boost::signals2::signal<void(int)> sig_key_down;

namespace
{
	bool ready_ = false;

	int width_ = 0;
	int height_ = 0;

	int mouse_x_ = 0;
	int mouse_y_ = 0;
	int mouse_btn_ = 0;

	std::unordered_map<int, bool> key_map_;

	void reshape(int width, int height)
	{
		width_ = width;
		height_ = height;

		glViewport(0, 0, width_, height_);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width_, 0, height_, -1, 1);

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
		ready_ = false;

		imgui_glut_shutdown();
		sig_win_close();
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
		if(27 /* Escape */ == key)
		{
			glutLeaveMainLoop();
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

			key_map_[key] = true;
			sig_key_down(key);
		}
	}

	void normal_key_up(unsigned char key, int x, int y)
	{
		if(!isprint(key))
		{
			ImGuiIO& io = ImGui::GetIO();
			io.KeysDown[key] = false;
		}

		key_map_[key] = false;
		sig_key_up(key);
	}

	void special_key_down(int key, int x, int y)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = true;

		key_map_[key] = true;
		sig_key_down(key);
	}

	void special_key_up(int key, int x, int y)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = false;

		key_map_[key] = false;
		sig_key_up(key);
	}
}

bool window_t::ready()
{
	return ready_;
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

bool window_t::key_down(int key)
{
	return key_map_[key];
}

void window_t::create(int width, int height, int color)
{
	int argc = 1;
	char* argv[] = { (char*)"", 0 };

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	width_ = width;
	height_ = height;

	int x = (glutGet(GLUT_SCREEN_WIDTH) - width_) / 2;
	int y = (glutGet(GLUT_SCREEN_HEIGHT) - height_) / 2;

	glutInitWindowSize(width_, height_);
	glutInitWindowPosition(x, y);

	glutCreateWindow("");

	glClearColor(RED(color), GREEN(color), BLUE(color), 1.0);

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

	ready_ = true;

	sig_win_create(width_, height_);

	glutMainLoop();
}

void window_t::close()
{
	glutLeaveMainLoop();
}
