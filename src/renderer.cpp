
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

////////////////////////////////////////////////////////////////////////////////

struct mouse_state_t
{
	mouse_state_t() : x_(0), y_(0), btn_(-1) {}

	int x_, y_, btn_;
};

static int window_width = 0;
static int window_height = 0;
static mouse_state_t mouse_state;

static void stop()
{
	imgui_glut_shutdown();
	glutLeaveMainLoop();
}

static void reshape(int width, int height)
{
	window_width = width;
	window_height = height;

	glViewport(0, 0, window_width, window_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, window_width, 0, window_height, -1, 1);

	glMatrixMode(GL_MODELVIEW);
}

static void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	imgui_glut_prepare(window_width, window_height);

	sig_renderer_render();

	// ImGui::ShowTestWindow();
	ImGui::Render();

	glutSwapBuffers();
}

static void idle()
{
	display();
}

static void normal_key_down(unsigned char key, int x, int y)
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

static void normal_key_up(unsigned char key, int x, int y)
{
	if(!isprint(key))
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = false;
	}
}

static void special_key_down(int key, int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[key] = true;
}

static void special_key_up(int key, int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[key] = false;
}

static void mouse_wheel(int dir, int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2(x, y);
	io.MouseWheel = dir;
}

static void mouse_click(int btn, int state, int x, int y)
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

static void mouse_move(int x, int y)
{
	mouse_state.x_ = x;
	mouse_state.y_ = window_height - 1 - y;
	mouse_state.btn_ = -1;

	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2(x, y);
}

static void mouse_drag(int x, int y)
{
	mouse_state.x_ = x;
	mouse_state.y_ = window_height - 1 - y;

	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2(x, y);
}

////////////////////////////////////////////////////////////////////////////////

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

void renderer_t::start(int width, int height, int bg_color)
{
	window_width = width;
	window_height = height;

	int argc = 1;
	char _[] = "";
	char* argv[] = { _, 0 };

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("");

#ifdef __APPLE__
	GLint swap_interval = 0;
	CGLContextObj ctx = CGLGetCurrentContext();

	if(ctx)
	{
		CGLSetParameter(ctx, kCGLCPSwapInterval, &swap_interval);
	}
#endif

	if(!(window_width > 0 && window_height > 0))
	{
		glutFullScreen();
	}
	else
	{
		glutPositionWindow((glutGet(GLUT_SCREEN_WIDTH) - window_width) / 2,
			(glutGet(GLUT_SCREEN_HEIGHT) - window_height) / 2);
	}

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

	double red = (bg_color / 65536 % 256) / 255.0;
	double green = (bg_color / 256 % 256) / 255.0;
	double blue = (bg_color % 256) / 255.0;

	glClearColor(red, green, blue, 1.0);

	imgui_glut_init();

	sig_renderer_start(window_width, window_height);

	glutMainLoop();

	sig_renderer_stop();
}

void renderer_t::stop()
{
	::stop();
}
