
#include "obb.h"

#include <cmath>

obb_t::obb_t() : extent_(0.0f, 0.0f)
{
	axis_[0].x = axis_[0].y = 0.0f;
	axis_[1].x = axis_[1].y = 0.0f;
}

obb_t::~obb_t()
{}

void obb_t::set_axis(const glm::vec2& axis)
{
	axis_[0] = axis;
	axis_[0] = glm::normalize(axis_[0]);

	axis_[1].x = axis_[0].y;
	axis_[1].y = -axis_[0].x;
}

void obb_t::set_extent(const glm::vec2& extent)
{
	extent_ = extent;

	extent_.x = abs(extent_.x);
	extent_.y = abs(extent_.y);
}

int obb_t::axis_num() const
{
	return sizeof(axis_) / sizeof(axis_[0]);
}

glm::vec2 obb_t::get_axis(int index) const
{
	return axis_[index];
}

glm::vec2 obb_t::get_extent() const
{
	return extent_;
}

void obb_t::project(const glm::vec2& axis, float& left, float& right) const
{
	float pos, radius;

	pos = glm::dot(pos_, axis);
	radius = abs(glm::dot(axis_[0], axis)) * extent_[0] +
		abs(glm::dot(axis_[1], axis)) * extent_[1];

	left = pos - radius;
	right = pos + radius;
}

void obb_t::rotate(float angle)
{
	glm::vec2 tmp;

	angle = angle / 180.0f * M_PI;

	float sin_value = sinf(angle);
	float cos_value = cosf(angle);

	tmp.x = axis_[0].x * cos_value + axis_[0].y * sin_value;
	tmp.y = -axis_[0].x * sin_value + axis_[0].y * cos_value;

	tmp = glm::normalize(tmp);

	axis_[0] = tmp;

	axis_[1].x = axis_[0].y;
	axis_[1].y = -axis_[0].x;
}

void obb_t::rotate(const glm::vec2& point, float angle)
{
	glm::vec2 tmp = pos_ - point;

	angle = angle / 180.0f * M_PI;

	float sin_value = sinf(angle);
	float cos_value = cosf(angle);

	pos_.x = point.x + tmp.x * cos_value + tmp.y * sin_value;
	pos_.y = point.y - tmp.x * sin_value + tmp.y * cos_value;

	tmp.x = axis_[0].x * cos_value + axis_[0].y * sin_value;
	tmp.y = -axis_[0].x * sin_value + axis_[0].y * cos_value;

	tmp = glm::normalize(tmp);

	axis_[0] = tmp;

	axis_[1].x = axis_[0].y;
	axis_[1].y = -axis_[0].x;
}
