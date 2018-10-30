
#ifndef INDEX_GRID_INCLUDED
#define INDEX_GRID_INCLUDED

#include <vector>

#include "fluid_particle.h"

class index_grid_t
{
public:
	void init(float cell_space, float sim_width, float sim_height);

	float get_cell_space() const;

	int get_width() const;
	int get_height() const;

	std::vector<int> get_neighbor(fluid_particle_t* particle) const;

	void refresh(const std::vector<fluid_particle_t*>& particles);
	void adjust(const std::vector<fluid_particle_t*>& particles);

private:
	float cell_space_;
	int width_, height_;

	std::vector<std::vector<std::vector<int>>> grid_;
};

#endif
