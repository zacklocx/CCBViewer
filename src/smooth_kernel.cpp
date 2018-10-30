
#include "smooth_kernel.h"

smooth_kernel_t::smooth_kernel_t() : h_(0.0f), r_(0.0f), v_(0.0f, 0.0f)
{}

smooth_kernel_t::~smooth_kernel_t()
{}

void smooth_kernel_t::set_h(float h)
{
	h_ = h;
}

float smooth_kernel_t::get_h() const
{
	return h_;
}

void smooth_kernel_t::input_v(const glm::vec2& v)
{
	v_ = v;
	r_ = v_.length();
}
