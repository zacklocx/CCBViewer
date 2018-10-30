
#ifndef COLLISION_RESOLVER_INCLUDED
#define COLLISION_RESOLVER_INCLUDED

#include <vector>

#include "fluid_particle.h"

#include "obb.h"
#include "point_volume.h"

inline bool collision_solve(obb_t& obb, const std::vector<fluid_particle_t*> particles, float bounciness, float friction)
{
	bool collided = false;

	point_volume_t point_volume;
	point_volume.set_margin(cell_space * 0.25f);

	glm::vec2 normal, tangent, penetration;
	float length;

	glm::vec2 v, vn, vt;

	for(size_t i = 0; i < particles.size(); ++i)
	{
		point_volume.set_position(particles[i]->get_position());

		if(bounding_volume_t::intersects(obb, point_volume, normal, length))
		{
			tangent.x = normal.y;
			tangent.y = -normal.x;

			penetration = normal * length;

			if(point_volume.is_fixed())
			{
				obb.set_position(obb.get_position() + penetration);
			}
			else
			{
				v = particles[i]->get_position() - particles[i]->get_prev_position();
				vn = -normal * glm::dot(v, normal) * bounciness;
				vt = tangent * glm::dot(v, tangent) * (1.0f - friction);
				v = vn + vt;

				particles[i]->set_position(particles[i]->get_position() - penetration);
				particles[i]->set_prev_position(particles[i]->get_position() - v);
			}
		}
	}

	return collided;	
}

#endif /* COLLISION_RESOLVER_INCLUDED */
