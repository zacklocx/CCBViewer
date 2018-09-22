
#include "window.h"

#include <cctype>

#include <unordered_map>

#include <GL/freeglut.h>

#include "imgui.h"
#include "imgui_glut.h"

boost::signals2::signal<void()> sig_win_create;
boost::signals2::signal<void()> sig_win_render;
boost::signals2::signal<void()> sig_win_destroy;
boost::signals2::signal<void(int, int)> sig_win_resize;

boost::signals2::signal<void(int)> sig_key_up;
boost::signals2::signal<void(int)> sig_key_down;

boost::signals2::signal<void(int, int)> sig_mouse_move;
boost::signals2::signal<void(int, int, int)> sig_mouse_up;
boost::signals2::signal<void(int, int, int)> sig_mouse_down;
boost::signals2::signal<void(int, int, int)> sig_mouse_drag;
boost::signals2::signal<void(int, int, int)> sig_mouse_wheel;

namespace
{
	enum class win_state_t { halted, working, halting };

	win_state_t win_state = win_state_t::halted;

	int win_width = 0;
	int win_height = 0;

	std::unordered_map<int, bool> key_map;

	int mouse_x = 0;
	int mouse_y = 0;
	int mouse_btn = 0;
	int mouse_dir = 0;

	void reshape(int width, int height)
	{
		win_width = width;
		win_height = height;

		glViewport(0, 0, win_width, win_height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, win_width, 0, win_height, -1, 1);

		glMatrixMode(GL_MODELVIEW);

		sig_win_resize(win_width, win_height);
	}

	void display()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		imgui_glut_prepare(win_width, win_height);
		sig_win_render();
		ImGui::Render();

		glutSwapBuffers();
	}

	void idle()
	{
		(win_state != win_state_t::halting)? glutPostRedisplay() : glutLeaveMainLoop();
	}

	void close()
	{
		win_state = win_state_t::halted;

		imgui_glut_shutdown();
		sig_win_destroy();
	}

	void normal_key_down(unsigned char key, int x, int y)
	{
		if(27 /* Escape */ == key)
		{
			win_state = win_state_t::halting;
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

		key_map[key] = true;
		sig_key_down(key);
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

	void mouse_move(int x, int y)
	{
		mouse_x = x;
		mouse_y = win_height - 1 - y;
		mouse_btn = -1;

		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(x, y);

		sig_mouse_move(mouse_x, mouse_y);
	}

	void mouse_click(int btn, int state, int x, int y)
	{
		bool left_btn = GLUT_LEFT_BUTTON == btn;
		bool right_btn = GLUT_RIGHT_BUTTON == btn;

		bool btn_up = GLUT_UP == state;
		bool btn_down = GLUT_DOWN == state;

		mouse_x = x;
		mouse_y = win_height - 1 - y;
		mouse_btn = left_btn? 0 : right_btn? 1 : -1;

		ImGuiIO& io = ImGui::GetIO();

		io.MousePos = ImVec2(x, y);
		io.MouseDown[0] = left_btn && btn_down;
		io.MouseDown[1] = right_btn && btn_down;

		if(btn_up)
		{
			sig_mouse_up(mouse_btn, mouse_x, mouse_y);
		}
		else if(btn_down)
		{
			sig_mouse_down(mouse_btn, mouse_x, mouse_y);
		}
	}

	void mouse_drag(int x, int y)
	{
		mouse_x = x;
		mouse_y = win_height - 1 - y;

		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(x, y);

		sig_mouse_drag(mouse_btn, mouse_x, mouse_y);
	}

	void mouse_wheel(int dir, int x, int y)
	{
		mouse_x = x;
		mouse_y = win_height - 1 - y;

		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(x, y);
		io.MouseWheel = dir;

		sig_mouse_wheel(dir, mouse_x, mouse_y);
	}
}

bool window_t::ready()
{
	return win_state_t::working == win_state;
}

int window_t::width()
{
	return win_width;
}

int window_t::height()
{
	return win_height;
}

bool window_t::key_up(int key)
{
	return !key_map[key];
}

bool window_t::key_down(int key)
{
	return key_map[key];
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

int window_t::mouse_dir()
{
	return ::mouse_dir;
}

void window_t::create(int width, int height, int color)
{
	int argc = 1;
	char _[] = "";
	char* argv[] = { _, 0 };

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	int screen_width = glutGet(GLUT_SCREEN_WIDTH);
	int screen_height = glutGet(GLUT_SCREEN_HEIGHT);

	win_width = (width > 0)? width : screen_width;
	win_height = (height > 0)? height : screen_height;

	glutInitWindowSize(win_width, win_height);
	glutInitWindowPosition((screen_width - win_width) / 2, (screen_height - win_height) / 2);

	glutCreateWindow("");

	glClearColor(((color & 0xFF0000) >> 16) / 255.0,
				 ((color & 0x00FF00) >> 8) / 255.0,
				 ((color & 0x0000FF) / 255.0),
				 1.0);

	imgui_glut_init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutCloseFunc(close);

	glutKeyboardFunc(normal_key_down);
	glutKeyboardUpFunc(normal_key_up);
	glutSpecialFunc(special_key_down);
	glutSpecialUpFunc(special_key_up);

	glutMouseFunc(mouse_click);
	glutPassiveMotionFunc(mouse_move);
	glutMotionFunc(mouse_drag);

	win_state = win_state_t::working;

	sig_win_create();

	glutMainLoop();
}

void window_t::destroy()
{
	win_state = win_state_t::halting;
}
