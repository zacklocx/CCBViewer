
#include "bounding_volume.h"

#include <cmath>

bounding_volume_t::bounding_volume_t() : is_fixed_(false), margin_(0.0f), pos_(0.0f, 0.0f)
{}

bounding_volume_t::~bounding_volume_t()
{}

void bounding_volume_t::set_fixed(bool is_fixed)
{
	is_fixed_ = is_fixed;
}

void bounding_volume_t::set_margin(float margin)
{
	margin_ = margin;
}

void bounding_volume_t::set_position(const glm::vec2& pos)
{
	pos_ = pos;
}

bool bounding_volume_t::is_fixed() const
{
	return is_fixed_;
}

float bounding_volume_t::get_margin() const
{
	return margin_;
}

glm::vec2 bounding_volume_t::get_position() const
{
	return pos_;
}

bool bounding_volume_t::test_separating_axis(const bounding_volume_t& this_volume,
	const bounding_volume_t& that_volume,
	const glm::vec2& axis,
	float& this_left, float& this_right,
	float& that_left, float& that_right)
{
	this_volume.project(axis, this_left, this_right);
	this_left -= this_volume.margin_;
	this_right += this_volume.margin_;

	that_volume.project(axis, that_left, that_right);
	that_left -= that_volume.margin_;
	that_right += that_volume.margin_;

	return this_left >= that_right || that_left >= this_right;
}

bool bounding_volume_t::find_least_penetrating(const bounding_volume_t& this_volume,
	const bounding_volume_t& that_volume,
	const glm::vec2& axis,
	glm::vec2& normal, float& length)
{
	float this_left, this_right, that_left, that_right;

	if(test_separating_axis(this_volume, that_volume, axis, this_left, this_right, that_left, that_right))
	{
		return false;
	}

	float diff = fmin(this_right, that_right) - fmax(this_left, that_left);

	if(length < 0.0f || diff < length)
	{
		normal = axis;
		length = diff;
	}

	return true;
}

bool bounding_volume_t::intersects(const bounding_volume_t& this_volume,
	const bounding_volume_t& that_volume,
	glm::vec2& normal, float& length)
{
	normal.x = normal.y = 0.0f;
	length = -1.0f;

	int axis_num;

	axis_num = this_volume.axis_num();

	for(int i = 0; i < axis_num; ++i)
	{
		if(!find_least_penetrating(this_volume, that_volume, this_volume.get_axis(i), normal, length))
		{
			return false;
		}
	}

	axis_num = this_volume.axis_num();

	for(int i = 0; i < axis_num; ++i)
	{
		if(!find_least_penetrating(this_volume, that_volume, that_volume.get_axis(i), normal, length))
		{
			return false;
		}
	}

	if(glm::dot(this_volume.pos_ - that_volume.pos_, normal) < 0.0f)
	{
		normal *= -1.0f;
	}

	return true;
}
