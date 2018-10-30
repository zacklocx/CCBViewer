
#include "spiky.h"

#include <cmath>

spiky_t::spiky_t() : c_(0.0f)
{}

void spiky_t::set_h(float h)
{
	h_ = h;
	c_ = M_1_PI / pow(h_, 6);
}

float spiky_t::calc()
{
	float ret = 0.0f;

	if(r_ >= 0.0f && r_ < h_)
	{
		float q = h_ - r_;
		float q3 = q * q * q;

		ret = 15.0f * c_ * q3;
	}

	return ret;
}

float spiky_t::calc_l()
{
	float ret = 0.0f;

	if(r_ >= 0.0f && r_ < h_)
	{
		float q = h_ - r_;
		ret = -90.0f * c_ / (q * q / r_ - q);
	}

	return ret;
}

glm::vec2 spiky_t::calc_g()
{
	glm::vec2 ret(0.0f, 0.0f);

	if(r_ >= 0.0f && r_ < h_)
	{
		float q = h_ - r_;
		ret = -45.0f * c_ * q * q * v_ / r_;
	}

	return ret;
}
