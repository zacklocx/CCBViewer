
#include "poly6.h"

#include <cmath>

poly6_t::poly6_t() : c_(0.0f)
{}

void poly6_t::set_h(float h)
{
	h_ = h;
	c_ = M_1_PI / pow(h_, 9);
}

float poly6_t::calc()
{
	float ret = 0.0f;

	if(r_ >= 0.0f && r_ < h_)
	{
		float q = h_ * h_ - r_ * r_;
		float q3 = q * q * q;

		ret = (315.0f / 64.0f) * c_ * q3;
	}

	return ret;
}

float poly6_t::calc_l()
{
	float ret = 0.0f;

	if(r_ >= 0.0f && r_ < h_)
	{
		float q = h_ * h_ - r_ * r_;
		ret = -(945.0f / 32.0f) * c_ * (3.0f * q * q - 4.0f * r_ * r_ * q);
	}

	return ret;
}

glm::vec2 poly6_t::calc_g()
{
	glm::vec2 ret(0.0f, 0.0f);

	if(r_ >= 0.0f && r_ < h_)
	{
		float q = r_ / h_;
		ret = -(945.0f / 32.0f) * c_ * q * q * v_;
	}

	return ret;
}
