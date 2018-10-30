
#include "point_volume.h"

int point_volume_t::axis_num() const
{
	return 0;
}

glm::vec2 point_volume_t::get_axis(int index) const
{
	return glm::vec2(0.0f, 0.0f);
}

void point_volume_t::project(const glm::vec2& axis, float& left, float& right) const
{
	left = right = glm::dot(pos_, axis);
}
