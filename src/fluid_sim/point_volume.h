
#ifndef POINT_VOLUME_INCLUDED
#define POINT_VOLUME_INCLUDED

#include <glm/glm.hpp>

#include "bounding_volume.h"

class point_volume_t : public bounding_volume_t
{
public:
	int axis_num() const;
	glm::vec2 get_axis(int index) const;

	void project(const glm::vec2& axis, float& left, float& right) const;
};

#endif /* POINT_VOLUME_INCLUDED */
