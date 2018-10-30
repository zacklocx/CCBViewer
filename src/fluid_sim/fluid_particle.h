
#ifndef FLUID_PARTICLE_INCLUDED
#define FLUID_PARTICLE_INCLUDED

#include <glm/glm.hpp>

class fluid_particle_t
{
public:
	fluid_particle_t();
	~fluid_particle_t();

	void set_life(float l);
	void set_mass(float m);
	void set_density(float d);

	void calc_pressure();

	void set_position(const glm::vec2& p);
	void set_prev_position(const glm::vec2& p);

	void set_velocity(const glm::vec2& v);
	void set_force(const glm::vec2& f);

	float get_life() const;
	float get_mass() const;
	float get_density() const;
	float get_pressure() const;

	glm::vec2 get_position() const;
	glm::vec2 get_prev_position() const;

	glm::vec2 get_velocity() const;
	glm::vec2 get_force() const;

	void update(float dt, float max_force = 0.0f);

private:
	float life_, mass_, density_, pressure_;
	glm::vec2 pos_, prev_pos_, velocity_, force_;
};

#endif /* FLUID_PARTICLE_INCLUDED */
