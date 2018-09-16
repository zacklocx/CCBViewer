
#include "demo.h"

#include <GL/freeglut.h>

#include "llog.h"
#include "window.h"

demo_t::demo_t()
	: x_(0.0f), y_(0.0f)
{
}

void demo_t::draw() const
{
	// glPushMatrix();
	// glLoadIdentity();

	// glColor3ub(255, 0, 0);

	// glBegin(GL_TRIANGLES);
	// 	glVertex2f(0, 0);
	// 	glVertex2f(250, 500);
	// 	glVertex2f(500, 500);
	// glEnd();

	// glColor3ub(255, 255, 255);

	// glPopMatrix();

	if(0 == tex_.id())
	{
		if(const_cast<texture_t&>(tex_).load("/Users/zacklocx/tmp/tex.jpg"))
		{
			LLOG("tex_id") << tex_.id();
		}
		else
		{
			LLOG("fail load tex");
		}
	}
	else
	{
		tex_.draw(window_t::width() / 2, window_t::height() / 2, tex_.width(), tex_.height());
	}
}

void render(const demo_t& d)
{
	d.draw();
}
