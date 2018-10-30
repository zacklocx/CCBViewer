
#include "viscosity.h"

#include <cmath>

viscosity_t::viscosity_t() : c_(0.0f)
{}

void viscosity_t::set_h(float h)
{
	h_ = h;
	c_ = M_1_PI / pow(h_, 3);
}

float viscosity_t::calc()
{
	float ret = 0.0f;

	if(r_ >= 0.0f && r_ < h_)
	{
		float q = r_ / h_;
		float q3 = q * q * q;

		ret = (15.0f / 2.0f) * c_ * (-0.5f * q3 + q * q + 0.5f / q - 1.0f);
	}

	return ret;
}

float viscosity_t::calc_l()
{
	float ret = 0.0f;

	if(r_ >= 0.0f && r_ < h_)
	{
		ret = 45.0f * c_ / (h_ * h_ * h_) * (h_ - r_);
	}

	return ret;
}

glm::vec2 viscosity_t::calc_g()
{
	glm::vec2 ret(0.0f, 0.0f);

	if(r_ >= 0.0f && r_ < h_)
	{
		float q = r_ / h_;
		float q3 = q * q * q;

		ret = (15.0f / 2.0f) * c_ / (h_ * h_) * (-1.5f * q + 2.0f - 0.5f / q3) * v_;
	}

	return ret;
}
