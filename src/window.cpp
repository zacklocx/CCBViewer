
#include "window.h"

#include <cctype>

#include <unordered_map>

#include <GL/freeglut.h>

#include "imgui.h"
#include "imgui_glut.h"

#define RED(color) ((color & 0xFF0000) >> 16)
#define GREEN(color) ((color & 0x00FF00) >> 8)
#define BLUE(color) (color & 0x0000FF)

boost::signals2::signal<void()> sig_win_create;
boost::signals2::signal<void()> sig_win_render;
boost::signals2::signal<void()> sig_win_destroy;
boost::signals2::signal<void(int, int)> sig_win_resize;

boost::signals2::signal<void(int, int)> sig_mouse_move;
boost::signals2::signal<void(int, int, int)> sig_mouse_up;
boost::signals2::signal<void(int, int, int)> sig_mouse_down;
boost::signals2::signal<void(int, int, int)> sig_mouse_drag;
boost::signals2::signal<void(int, int, int)> sig_mouse_wheel;

boost::signals2::signal<void(int)> sig_key_up;
boost::signals2::signal<void(int)> sig_key_down;

namespace
{
	int width = 0;
	int height = 0;

	int mouse_x = 0;
	int mouse_y = 0;
	int mouse_btn = 0;

	std::unordered_map<int, bool> key_map;

	void reshape(int w, int h)
	{
		width = w;
		height = h;

		glViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width, 0, height, -1, 1);

		glMatrixMode(GL_MODELVIEW);

		sig_win_resize(width, height);
	}

	void display()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		imgui_glut_prepare(width, height);
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
		imgui_glut_shutdown();
		sig_win_destroy();
	}

	void mouse_move(int x, int y)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(x, y);

		mouse_x = x;
		mouse_y = height - 1 - y;
		mouse_btn = -1;

		sig_mouse_move(mouse_x, mouse_y);
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

				io.MouseWheel = dir;
				sig_mouse_wheel(mouse_x, mouse_y, dir);
			}
		}
		else
		{
			bool left_btn = GLUT_LEFT_BUTTON == btn;
			bool right_btn = GLUT_RIGHT_BUTTON == btn;

			mouse_btn = left_btn? 0 : right_btn? 1 : mouse_btn;

			if(0 == mouse_btn || 1 == mouse_btn)
			{
				if(btn_up)
				{
					io.MouseDown[mouse_btn] = false;
					sig_mouse_up(mouse_x, mouse_y, mouse_btn);
				}
				else if(btn_down)
				{
					io.MouseDown[mouse_btn] = true;
					sig_mouse_down(mouse_x, mouse_y, mouse_btn);
				}
			}
		}
	}

	void mouse_drag(int x, int y)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(x, y);

		mouse_x = x;
		mouse_y = height - 1 - y;

		sig_mouse_drag(mouse_x, mouse_y, mouse_btn);
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

			key_map[key] = true;
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

		key_map[key] = false;
		sig_key_up(key);
	}

	void special_key_down(int key, int x, int y)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = true;

		key_map[key] = true;
		sig_key_down(key);
	}

	void special_key_up(int key, int x, int y)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = false;

		key_map[key] = false;
		sig_key_up(key);
	}
}

int window_t::width()
{
	return ::width;
}

int window_t::height()
{
	return ::height;
}

int window_t::mouse_x()
{
	return ::mouse_x;
}

int window_t::mouse_y()
{
	return ::mouse_y;
}

int window_t::mouse_btn()
{
	return ::mouse_btn;
}

bool window_t::key_down(int key)
{
	return key_map[key];
}

void window_t::create(int width, int height, int color)
{
	int argc = 1;
	char _[] = "";
	char* argv[] = { _, 0 };

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(width, height);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - width) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - height) / 2);

	glutCreateWindow("");

	glClearColor(RED(color) / 255.0, GREEN(color) / 255.0, BLUE(color) / 255.0, 1.0);

	imgui_glut_init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutCloseFunc(close);

	glutMouseFunc(mouse_click);
	glutPassiveMotionFunc(mouse_move);
	glutMotionFunc(mouse_drag);

	glutKeyboardFunc(normal_key_down);
	glutKeyboardUpFunc(normal_key_up);
	glutSpecialFunc(special_key_down);
	glutSpecialUpFunc(special_key_up);

	sig_win_create();

	glutMainLoop();
}

void window_t::destroy()
{
	glutLeaveMainLoop();
}
