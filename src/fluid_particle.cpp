
#include "fluid_particle.h"

#include "verlet.h"
#include "sim_constants.h"

fluid_particle_t::fluid_particle_t()
{}

fluid_particle_t::~fluid_particle_t()
{}

void fluid_particle_t::set_life(float l)
{
	life_ = l;
}

void fluid_particle_t::set_mass(float m)
{
	mass_ = m;
}

void fluid_particle_t::set_density(float d)
{
	density_ = d;
}

void fluid_particle_t::calc_pressure()
{
	pressure_ = gas_k * (density_ - density_offset);
}

void fluid_particle_t::set_position(const glm::vec2& p)
{
	pos_ = p;
}

void fluid_particle_t::set_prev_position(const glm::vec2& p)
{
	prev_pos_ = p;
}

void fluid_particle_t::set_velocity(const glm::vec2& v)
{
	velocity_ = v;
}

void fluid_particle_t::set_force(const glm::vec2& f)
{
	force_ = f;
}

float fluid_particle_t::get_life() const
{
	return life_;
}

float fluid_particle_t::get_mass() const
{
	return mass_;
}

float fluid_particle_t::get_density() const
{
	return density_;
}

float fluid_particle_t::get_pressure() const
{
	return pressure_;
}

glm::vec2 fluid_particle_t::get_position() const
{
	return pos_;
}

glm::vec2 fluid_particle_t::get_prev_position() const
{
	return prev_pos_;
}

glm::vec2 fluid_particle_t::get_velocity() const
{
	return velocity_;
}

glm::vec2 fluid_particle_t::get_force() const
{
	return force_;
}

void fluid_particle_t::update(float dt, float max_force /* = 0.0f */)
{
	if(max_force > 0.0f)
	{
		float length = force_.length();

		if(length > max_force)
		{
			force_ *= max_force / length;
		}
	}

	verlet_solve(pos_, prev_pos_, velocity_, force_, mass_, 0.01f, dt);

	density_ = 0.0f;
	force_.x = force_.y = 0.0f;
}
