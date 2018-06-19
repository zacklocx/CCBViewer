
#include "render_win.h"

#include <cctype>

#include <GL/freeglut.h>

#include "imgui.h"
#include "imgui_glut.h"

boost::signals2::signal<void(int, int)> sig_win_create;
boost::signals2::signal<void()> sig_win_destroy;
boost::signals2::signal<void()> sig_win_render;

namespace
{
	enum class win_state_t { halted, working, halting };

	struct mouse_state_t
	{
		mouse_state_t() : x_(0), y_(0), btn_(-1) {}

		int x_, y_, btn_;
	};

	int win_width = 0;
	int win_height = 0;

	win_state_t win_state = win_state_t::halted;

	mouse_state_t mouse_state;

	void stop()
	{
		imgui_glut_shutdown();
		glutLeaveMainLoop();
	}

	void reshape(int width, int height)
	{
		win_width = width;
		win_height = height;

		glViewport(0, 0, win_width, win_height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, win_width, 0, win_height, -1, 1);

		glMatrixMode(GL_MODELVIEW);
	}

	void display()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		imgui_glut_prepare(win_width, win_height);

		sig_win_render();

		// ImGui::ShowTestWindow();
		ImGui::Render();

		glutSwapBuffers();
	}

	void idle()
	{
		if(win_state_t::halting == win_state)
		{
			stop();
			return;
		}

		display();
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
			mouse_state.y_ = win_height - 1 - y;
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
		mouse_state.y_ = win_height - 1 - y;
		mouse_state.btn_ = -1;

		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(x, y);
	}

	void mouse_drag(int x, int y)
	{
		mouse_state.x_ = x;
		mouse_state.y_ = win_height - 1 - y;

		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(x, y);
	}
}

bool render_win_t::ready()
{
	return win_state_t::working == win_state;
}

int render_win_t::width()
{
	return win_width;
}

int render_win_t::height()
{
	return win_height;
}

int render_win_t::mouse_x()
{
	return mouse_state.x_;
}

int render_win_t::mouse_y()
{
	return mouse_state.y_;
}

int render_win_t::mouse_btn()
{
	return mouse_state.btn_;
}

void render_win_t::create(int width, int height)
{
	int argc = 1;
	char _[] = "";
	char* argv[] = { _, 0 };

	glutInit(&argc, argv);

	int screen_width = glutGet(GLUT_SCREEN_WIDTH);
	int screen_height = glutGet(GLUT_SCREEN_HEIGHT);

	win_width = (width > 0)? width : screen_width;
	win_height = (height > 0)? height : screen_height;

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	// glutInitDisplayString( "rgba depth double samples>=8 stencil");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(win_width, win_height);
	glutCreateWindow("");
	glutPositionWindow((screen_width - win_width) / 2, (screen_height - win_height) / 2);

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

	int i;
	glGetIntegerv(GL_STENCIL_BITS, &i);
	std::cout<<"GL_STENCIL_BITS: "<<i<<std::endl;

	win_state = win_state_t::working;

	sig_win_create(win_width, win_height);

	glutMainLoop();

	win_state = win_state_t::halted;

	sig_win_destroy();
}

void render_win_t::destroy()
{
	win_state = win_state_t::halting;
}
