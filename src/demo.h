
#ifndef DEMO_INCLUDE
#define DEMO_INCLUDE

#include "texture.h"

class demo_t
{
public:
	demo_t();

	void set_x(float x) { x_ = x; }
	void set_y(float y) { y_ = y; }

	float get_x() const { return x_; }
	float get_y() const { return y_; }

	void draw() const;

private:
	float x_, y_;
	texture_t tex_;
};

void render(const demo_t&);

#endif /* DEMO_INCLUDED */
