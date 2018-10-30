
#ifndef OBB_INCLUDED
#define OBB_INCLUDED

#include <glm/glm.hpp>

#include "bounding_volume.h"

class obb_t : public bounding_volume_t
{
public:
	obb_t();
	~obb_t();

	void set_axis(const glm::vec2& axis);
	void set_extent(const glm::vec2& extent);

	int axis_num() const;
	glm::vec2 get_axis(int index) const;

	glm::vec2 get_extent() const;

	void project(const glm::vec2& axis, float& left, float& right) const;

	void rotate(float angle);
	void rotate(const glm::vec2& point, float angle);

private:
	glm::vec2 axis_[2], extent_;
};

#endif /* OBB_INCLUDED */
