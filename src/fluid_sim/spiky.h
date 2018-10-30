
#ifndef SPIKY_INCLUDED
#define SPIKY_INCLUDED

#include <glm/glm.hpp>

#include "smooth_kernel.h"

class spiky_t : public smooth_kernel_t
{
public:
	spiky_t();

	void set_h(float h);

	float calc();
	float calc_l();
	glm::vec2 calc_g();

private:
	float c_;
};

#endif /* SPIKY_INCLUDED */
