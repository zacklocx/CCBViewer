
#ifndef SMOOTH_KERNEL_INCLUDED
#define SMOOTH_KERNEL_INCLUDED

#include <glm/glm.hpp>

class smooth_kernel_t
{
public:
	smooth_kernel_t();
	virtual ~smooth_kernel_t();

	virtual void set_h(float h);
	virtual float get_h() const;

	virtual void input_v(const glm::vec2& v);

	virtual float calc() = 0;
	virtual float calc_l() = 0;
	virtual glm::vec2 calc_g() = 0;

protected:
	float h_, r_;
	glm::vec2 v_;
};

#endif
