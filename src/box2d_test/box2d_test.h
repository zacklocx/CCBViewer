#ifndef BOX2D_TEST_INCLUDED
#define BOX2D_TEST_INCLUDED

#include <vector>

#include "Box2D/Box2D.h"

#include "texture.h"

class box2d_test_t
{
public:
	box2d_test_t();

	void init();
	void render();
	void update();

private:
	float ground_hw_, ground_hh_;
	float box_hw_, box_hh_;

	b2World world_;
	std::vector<b2Body*> bodys_;

	texture_t texture_;
};

#endif
