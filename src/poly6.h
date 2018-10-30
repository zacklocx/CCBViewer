
#ifndef POLY6_INCLUDED
#define POLY6_INCLUDED

#include <glm/glm.hpp>

#include "smooth_kernel.h"

class poly6_t : public smooth_kernel_t
{
public:
	poly6_t();

	void set_h(float h);

	float calc();
	float calc_l();
	glm::vec2 calc_g();

private:
	float c_;
};

#endif /* POLY6_INCLUDED */
