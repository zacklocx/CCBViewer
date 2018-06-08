
#include "demo.h"

#include <GL/freeglut.h>

void render(const demo_t& d)
{
	glPushMatrix();
	glLoadIdentity();

	glColor3ub(255, 0, 0);

	glBegin(GL_TRIANGLES);

	glVertex2f(0, 0);
	glVertex2f(250, 500);
	glVertex2f(500, 500);

	glEnd();

	glPopMatrix();
}
