
#include "particle_emitter.h"

#include <random>

particle_emitter_t::particle_emitter_t()
{
	is_active_ = false;
	capacity_ = index_ = 0;

	dt_sum_ = frequency_ = distribution_ = 0.0f;

	position_ = direction_ = glm::vec2(0.0f, 0.0f);

	min_mass_ = max_mass_ = 0.0f;
	min_velocity_ = max_velocity_ = 0.0f;
}

particle_emitter_t::~particle_emitter_t()
{}

void particle_emitter_t::set_active(bool is_active)
{
	is_active_ = is_active;
}

void particle_emitter_t::set_capacity(int capacity)
{
	capacity_ = capacity;
}

void particle_emitter_t::set_index(int index)
{
	index_ = index;
}

void particle_emitter_t::set_frequency(float frequency)
{
	frequency_ = frequency;
}

void particle_emitter_t::set_distribution(float distribution)
{
	distribution_ = distribution;
}

void particle_emitter_t::set_position(const glm::vec2& position)
{
	position_ = position;
}

void particle_emitter_t::set_direction(const glm::vec2& direction)
{
	direction_ = direction;
}

void particle_emitter_t::set_mass_interval(float min_mass, float max_mass)
{
	min_mass_ = min_mass;
	max_mass_ = max_mass;
}

void particle_emitter_t::set_velocity_interval(float min_velocity, float max_velocity)
{
	min_velocity_ = min_velocity;
	max_velocity_ = max_velocity;
}

bool particle_emitter_t::is_active() const
{
	return is_active_;
}

int particle_emitter_t::get_capacity() const
{
	return capacity_;
}

int particle_emitter_t::get_index() const
{
	return index_;
}

float particle_emitter_t::get_frequency() const
{
	return frequency_;
}

float particle_emitter_t::get_distribution() const
{
	return distribution_;
}

glm::vec2 particle_emitter_t::get_position() const
{
	return position_;
}

glm::vec2 particle_emitter_t::get_direction() const
{
	return direction_;
}

float particle_emitter_t::get_min_mass() const
{
	return min_mass_;
}

float particle_emitter_t::get_max_mass() const
{
	return max_mass_;
}

float particle_emitter_t::get_min_velocity() const
{
	return min_velocity_;
}

float particle_emitter_t::get_max_velocity() const
{
	return max_velocity_;
}

std::vector<fluid_particle_t*> particle_emitter_t::emite(float dt)
{
	std::vector<fluid_particle_t*> ret;

	if(!is_active_ || (capacity_ > 0 && index_ >= capacity_))
	{
		return ret;
	}

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> dis(-0.5, 0.5);
	std::uniform_real_distribution<> mass_dis(min_mass_, max_mass_);
	std::uniform_real_distribution<> velocity_dis(min_velocity_, max_velocity_);

	dt_sum_ += dt;

	int num;
	num = (int)(frequency_ * dt_sum_);

	if(num > 0)
	{
		float dist;
		glm::vec2 normal, new_direction;

		float mass, velocity;
		fluid_particle_t* particle;

		for(int i = 0; i < num; ++i)
		{
			dist = dis(gen) * distribution_;

			normal.x = direction_.y;
			normal.y = -direction_.x;

			normal *= dist;

			new_direction = direction_ + normal;
			new_direction = glm::normalize(new_direction);

			mass = mass_dis(gen);
			velocity = velocity_dis(gen);

			particle = new fluid_particle_t;

			particle->set_life(1.0f);

			particle->set_mass(mass);
			particle->set_velocity(new_direction * velocity);

			particle->set_position(position_);
			particle->set_prev_position(position_ - particle->get_velocity() * dt_sum_);

			ret.push_back(particle);

			if(capacity_ > 0)
			{
				++index_;
				if(index_ >= capacity_)
				{
					index_ = capacity_;
					break;
				}
			}
		}

		dt_sum_ = 0.0f;
	}

	return ret;
}
