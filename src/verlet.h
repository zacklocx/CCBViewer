
#ifndef VERLET_INCLUDED
#define VERLET_INCLUDED

#include <glm/glm.hpp>

inline void verlet_solve(glm::vec2& r, glm::vec2& pr, glm::vec2& v, const glm::vec2& f, float m, float dp, float dt)
{
	glm::vec2 df((1.0f - dp) * (r - pr));
	pr = r;
	r = r + df + f / m * dt * dt;
	v = (r - pr) / dt;
}

#endif
