
#include "index_grid.h"

#include <cmath>

#include <glm/glm.hpp>

index_grid_t::index_grid_t()
{
	cell_space_ = 0.0f;
	width_ = height_ = 0;
}

void index_grid_t::init(float cell_space, float sim_width, float sim_height)
{
	cell_space_ = cell_space;

	width_ = (int)(sim_width / cell_space_);
	height_ = (int)(sim_height / cell_space_);

	grid_.resize(width_);

	for(int w = 0; w < width_; ++w)
	{
		grid_[w].resize(height_);
	}
}

float index_grid_t::get_cell_space() const
{
	return cell_space_;
}

int index_grid_t::get_width() const
{
	return width_;
}

int index_grid_t::get_height() const
{
	return height_;
}

std::vector<int> index_grid_t::get_neighbor(fluid_particle_t* particle) const
{
	std::vector<int> ret;

	if(particle != NULL)
	{
		int x, y;
		glm::vec2 p;

		p = particle->get_position();

		x = (int)(p.x / cell_space_);
		y = (int)(p.y / cell_space_);

		if(x < 0)
		{
			x = 0;
		}
		else if(x >= width_)
		{
			x = width_ - 1;
		}

		if(y < 0)
		{
			y = 0;
		}
		else if(y >= height_)
		{
			y = height_ - 1;
		}

		for(int i = -1; i <= 1; ++i)
		{
			if(x + i < 0 || x + i >= width_)
			{
				continue;
			}

			for(int j = -1; j <= 1; ++j)
			{
				if(y + j < 0 || y + j >= height_)
				{
					continue;
				}

				const std::vector<int> & vec = grid_[x + i][y + j];
				ret.insert(ret.end(), vec.begin(), vec.end());
			}
		}
	}

	return ret;
}

void index_grid_t::refresh(const std::vector<fluid_particle_t*>& particles)
{
	for(size_t w = 0; w < grid_.size(); ++w)
	{
		for(size_t h = 0; h < grid_[w].size(); ++h)
		{
			grid_[w][h].clear();
		}
	}

	int x, y;
	glm::vec2 p;

	for(size_t i = 0; i < particles.size(); ++i)
	{
		p = particles[i]->get_position();

		x = (int)(p.x / cell_space_);
		y = (int)(p.y / cell_space_);

		if(x < 0)
		{
			x = 0;
		}
		else if(x >= width_)
		{
			x = width_ - 1;
		}

		if(y < 0)
		{
			y = 0;
		}
		else if(y >= height_)
		{
			y = height_ - 1;
		}

		grid_[x][y].push_back((int)i);
	}
}

void index_grid_t::adjust(const std::vector<fluid_particle_t*>& particles)
{
	float min_dist = cell_space_ * 0.5f;

	int k;

	float mag_square;
	glm::vec2 p1, p2, prev_p1, prev_p2, dist;

	for(size_t i = 0; i < particles.size(); ++i)
	{
		std::vector<int> neighbor_index = get_neighbor(particles[i]);

		for(size_t j = 0; j < neighbor_index.size(); ++j)
		{
			k = neighbor_index[j];

			if((int)i == k)
			{
				continue;
			}

			p1 = particles[i]->get_position();
			p2 = particles[k]->get_position();

			dist = p1 - p2;
			mag_square = dist.x * dist.x + dist.y * dist.y;

			if(mag_square < min_dist * min_dist)
			{
				prev_p1 = particles[i]->get_prev_position();
				prev_p2 = particles[k]->get_prev_position();

				if(mag_square != 0.0f)
				{
					float mag = sqrtf(mag_square);
					dist = dist / mag * (min_dist - mag) * 0.5f;
				}
				else
				{
					dist.x = 0.0f;
					dist.y = min_dist * 0.5f;
				}

				particles[i]->set_position(p1 + dist);
				particles[i]->set_prev_position(prev_p1 + dist);

				particles[k]->set_position(p2 - dist);
				particles[k]->set_prev_position(prev_p2 - dist);
			}
		}
	}
}
