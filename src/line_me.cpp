
#include "line_me.h"

#include <Gl/gl.h>

#include "window.h"

#include "draw.h"
#include "sim_constants.h"
#include "collision_resolver.h"

line_me_t::line_me_t()
{}

bool line_me_t::init()
{
	win_width_ = window_t::width();
	win_height_ = window_t::height();

	win_scale_x_ = sim_width / win_width_;
	win_scale_y_ = sim_height / win_height_;

	main_area_x0_ = cell_space * 0.5f;
	main_area_y0_ = cell_space * 0.5f;

	main_area_x1_ = sim_width * 0.75f - cell_space * 0.5f;
	main_area_y1_ = sim_height - cell_space * 0.5f;

	side_area_x0_ = main_area_x1_ + cell_space * 0.5f;
	side_area_y0_ = cell_space * 0.5f;

	side_area_x1_ = sim_width - cell_space * 0.5f;
	side_area_y1_ = sim_height - cell_space * 0.5f;

	max_force_ = 500.0f;

	index_grid_.init(cell_space, sim_width, sim_height);

	emitter_.set_active(true);
	emitter_.set_capacity(1000);
	emitter_.set_frequency(30);
	emitter_.set_distribution(0.01f);

	glm::vec2 position, direction, extent;

	position.x = main_area_y0_;
	position.y = main_area_y1_;

	direction.x = 0.8f;
	direction.y = -0.25f;

	emitter_.set_position(position);
	emitter_.set_direction(direction);

	emitter_.set_mass_interval(particle_mass, particle_mass);
	emitter_.set_velocity_interval(particle_mass * 0.45f, particle_mass * 0.5f);

	viscosity_.set_h(cell_space);

	ready_ = false;

	direction.x = 1.0f;
	direction.y = 0.0f;

	obb_locked_ = false;

	elastic_obb_.set_axis(direction);

	rot_speed_ = 6.0f;

	leak_ = true;
	show_edge_ = false;

	force_coeff_ = 0;
}

void line_me_t::resize(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(sim_left, sim_right, sim_bottom, sim_top, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void line_me_t::reset()
{
	particles_.clear();

	emitter_.set_index(0);
	emitter_.set_active(true);
}

void line_me_t::render()
{
	std::vector<fluid_particle_t*> ret;
	ret = emitter_.emite(delta_time_sec);
	particles_.insert(particles_.end(), ret.begin(), ret.end());

	index_grid_.refresh(particles_);

	fluid_particle_t* particle, * neighbor_particle;

	float density, pressure, dist;
	glm::vec2 p, prev_p, v, f, next_f;

	for(size_t i = 0; i < particles_.size(); ++i)
	{
		particle = particles_[i];

		density = particle->get_density();

		p = particle->get_position();

		std::vector<int> neighbor_index = index_grid_.get_neighbor(particle);

		for(size_t j = 0; j < neighbor_index.size(); ++j)
		{
			neighbor_particle = particles_[neighbor_index[j]];

			poly6_.input_v(p - neighbor_particle->get_position());
			density += particle_mass * poly6_.calc();
		}

		particle->set_density(density);
		particle->calc_pressure();
	}

	for(size_t i = 0; i < particles_.size(); ++i)
	{
		particle = particles_[i];

		f = particle->get_force();

		f += particle_mass * gravity;

		p = particle->get_position();
		v = particle->get_velocity();

		dist = (p - mouse_pos_).length();
		if(dist > 0.0f && dist < cell_space * 2.0f)
		{
			f += force_coeff_ * 1000.0f * (p - mouse_pos_) / dist;
		}

		pressure = particle->get_pressure();

		std::vector<int> neighbor_index = index_grid_.get_neighbor(particle);

		for(size_t j = 0; j < neighbor_index.size(); ++j)
		{
			if(i < (size_t)neighbor_index[j])
			{
				neighbor_particle = particles_[neighbor_index[j]];

				spiky_.input_v(p - neighbor_particle->get_position());
				next_f = -0.5f * particle_mass * (pressure + neighbor_particle->get_pressure()) /
					neighbor_particle->get_density() * spiky_.calc_g();

				f += next_f;
				neighbor_particle->set_force(neighbor_particle->get_force() - next_f);

				viscosity_.input_v(p - neighbor_particle->get_position());
				next_f = visco_coeff * particle_mass * (neighbor_particle->get_velocity() - v) /
					neighbor_particle->get_density() * viscosity_.calc_l();

				f += next_f;
				neighbor_particle->set_force(neighbor_particle->get_force() - next_f);
			}
		}

		particle->set_force(f);
	}

	use_color("blue");

	p = emitter_.get_position();
	draw_circle(p.x, p.y, 0.0f, 0.05f);

	use_color("red");
	draw_circle(mouse_pos_.x, mouse_pos_.y, 0.0f, 0.05f);

	glm::vec2 ex[2];

	if(selected_obb_.size() > 0)
	{
		use_color("lime");

		glBegin(GL_QUADS);

		for(size_t i = 0; i < edge_obb_.size(); ++i)
		{
			if(selected_obb_.count((int)i) > 0)
			{
				p = edge_obb_[i].get_position();

				ex[0] = edge_obb_[i].get_axis(0) * edge_obb_[i].get_extent().x;
				ex[1] = edge_obb_[i].get_axis(1) * edge_obb_[i].get_extent().y;

				glVertex2f(p.x - ex[0].x - ex[1].x, p.y - ex[0].y - ex[1].y);
				glVertex2f(p.x + ex[0].x - ex[1].x, p.y + ex[0].y - ex[1].y);
				glVertex2f(p.x + ex[0].x + ex[1].x, p.y + ex[0].y + ex[1].y);
				glVertex2f(p.x - ex[0].x + ex[1].x, p.y - ex[0].y + ex[1].y);
			}
		}

		glEnd();

		use_color("fuchsia");
		draw_circle(center_.x, center_.y, 0.0f, 0.05f);
	}

	use_color("purple");

	glBegin(GL_QUADS);

	for(size_t i = 0; i < draw_obb_.size(); ++i)
	{
		p = draw_obb_[i].get_position();

		ex[0] = draw_obb_[i].get_axis(0) * draw_obb_[i].get_extent().x;
		ex[1] = draw_obb_[i].get_axis(1) * draw_obb_[i].get_extent().y;

		glVertex2f(p.x - ex[0].x - ex[1].x, p.y - ex[0].y - ex[1].y);
		glVertex2f(p.x + ex[0].x - ex[1].x, p.y + ex[0].y - ex[1].y);
		glVertex2f(p.x + ex[0].x + ex[1].x, p.y + ex[0].y + ex[1].y);
		glVertex2f(p.x - ex[0].x + ex[1].x, p.y - ex[0].y + ex[1].y);
	}

	glEnd();

	ex[0] = elastic_obb_.get_extent();

	use_color("lime");

	p = elastic_obb_.get_position();

	glBegin(GL_LINE_LOOP);
		glVertex2f(p.x - ex[0].x, p.y - ex[0].y);
		glVertex2f(p.x + ex[0].x, p.y - ex[0].y);
		glVertex2f(p.x + ex[0].x, p.y + ex[0].y);
		glVertex2f(p.x - ex[0].x, p.y + ex[0].y);
	glEnd();

	use_color("blue");

	glBegin(GL_POINTS);

	for(size_t i = 0; i < particles_.size(); ++i)
	{
		particle = particles_[i];
		assert(particle != NULL);

		particle->update(delta_time_sec, max_force_);

		p = particle->get_position();

		if(!leak_)
		{
			if(p.x <= main_area_x0_ || p.x >= main_area_x1_ || p.y <= main_area_y0_ || p.y >= main_area_y1_)
			{
				if(p.x <= main_area_x0_)
				{
					p.x = sim_left + main_area_x0_;
				}
				else if(p.x >= main_area_x1_)
				{
					p.x = sim_right - main_area_x1_;
				}

				if(p.y <= main_area_y0_)
				{
					p.y = main_area_y0_;
				}
				else if(p.y >= main_area_y1_)
				{
					p.y = main_area_y1_;
				}

				particle->set_position(p);
			}
		}
		else
		{
			if(p.x <= main_area_x0_ || p.x >= main_area_x1_ || p.y <= main_area_y0_ || p.y >= main_area_y1_)
			{
				particle->set_life(0.0f);
			}
		}

		glVertex2f(p.x, p.y);
	}

	glEnd();

	use_color("black");

	glBegin(GL_LINE_LOOP);
		glVertex2f(main_area_x0_, main_area_y0_);
		glVertex2f(main_area_x1_, main_area_y0_);
		glVertex2f(main_area_x1_, main_area_y1_);
		glVertex2f(main_area_x0_, main_area_y1_);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex2f(side_area_x0_, side_area_y0_);
		glVertex2f(side_area_x1_, side_area_y0_);
		glVertex2f(side_area_x1_, side_area_y1_);
		glVertex2f(side_area_x0_, side_area_y1_);
	glEnd();

	use_color("white");
}

void line_me_t::idle()
{
	index_grid_.adjust(particles_);

	if(leak_)
	{
		std::vector<fluid_particle_t*>::iterator itor = particles_.begin();

		while(itor != particles_.end())
		{
			if(!((*itor)->get_life() > 0.0f))
			{
				delete *itor;
				itor = particles_.erase(itor);
			}
			else
			{
				++itor;
			}
		}

		emitter_.set_index((int)particles_.size());
	}

	for(size_t i = 0; i < draw_obb_.size(); ++i)
	{
		collision_solve(draw_obb_[i], particles_, 0.2f, 0.0f);
	}

	for(size_t i = 0; i < edge_obb_.size(); ++i)
	{
		collision_solve(edge_obb_[i], particles_, 0.2f, 0.0f);
	}
}
