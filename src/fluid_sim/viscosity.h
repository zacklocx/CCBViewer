
#ifndef VISCOSITY_INCLUDED
#define VISCOSITY_INCLUDED

#include <glm/glm.hpp>

#include "smooth_kernel.h"

class viscosity_t : public smooth_kernel_t
{
public:
	viscosity_t();

	void set_h(float h);

	float calc();
	float calc_l();
	glm::vec2 calc_g();

private:
	float c_;
};

#endif /* VISCOSITY_INCLUDED */
