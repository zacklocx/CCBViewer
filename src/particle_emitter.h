
#ifndef PARTICLE_EMITTER_INCLUDED
#define PARTICLE_EMITTER_INCLUDED

#include <vector>

#include <glm/glm.hpp>

#include "fluid_particle.h"

class particle_emitter_t
{
public:
	particle_emitter_t();
	~particle_emitter_t();

	void set_active(bool is_active);

	void set_capacity(int capacity);
	void set_index(int index);

	void set_frequency(float frequency);
	void set_distribution(float distribution);

	void set_position(const glm::vec2& position);
	void set_direction(const glm::vec2& direction);

	void set_mass_interval(float min_mass, float max_mass);
	void set_velocity_interval(float min_velocity, float max_velocity);

	bool is_active() const;

	int get_capacity() const;
	int get_index() const;

	float get_frequency() const;
	float get_distribution() const;

	glm::vec2 get_position() const;
	glm::vec2 get_direction() const;

	float get_min_mass() const;
	float get_max_mass() const;

	float get_min_velocity() const;
	float get_max_velocity() const;

	std::vector<fluid_particle_t*> emite(float dt);

private:
	bool is_active_;
	int capacity_, index_;

	float dt_sum_, frequency_, distribution_;

	glm::vec2 position_, direction_;

	float min_mass_, max_mass_;
	float min_velocity_, max_velocity_;
};

#endif
