
#ifndef BOUNDING_VOLUME_INCLUDED
#define BOUNDING_VOLUME_INCLUDED

#include <glm/glm.hpp>

class bounding_volume_t
{
public:
	bounding_volume_t();
	virtual ~bounding_volume_t();

	void set_fixed(bool is_fixed);
	void set_margin(float margin);
	void set_position(const glm::vec2& pos);

	bool is_fixed() const;
	float get_margin() const;
	glm::vec2 get_position() const;

	virtual int axis_num() const = 0;
	virtual glm::vec2 get_axis(int index) const = 0;
	virtual void project(const glm::vec2& axis, float& left, float& right) const = 0;

	static bool test_separating_axis(const bounding_volume_t& this_volume,
		const bounding_volume_t& that_volume,
		const glm::vec2& axis,
		float& this_left, float& this_right,
		float& that_left, float& that_right);

	static bool find_least_penetrating(const bounding_volume_t& this_volume,
		const bounding_volume_t& that_volume,
		const glm::vec2& axis,
		glm::vec2& normal, float& length);

	static bool intersects(const bounding_volume_t& this_volume,
		const bounding_volume_t& that_volume,
		glm::vec2& normal, float& length);

protected:
	bool is_fixed_;
	float margin_;
	glm::vec2 pos_;
};

#endif /* BOUNDING_VOLUME_INCLUDED */
