
#ifndef FLUID_SIM_INCLUDED
#define FLUID_SIM_INCLUDED

#include <vector>
#include <unordered_set>

#include <glm/glm.hpp>

#include "obb.h"
#include "index_grid.h"
#include "particle_emitter.h"

#include "poly6.h"
#include "spiky.h"
#include "viscosity.h"

class fluid_sim_t
{
public:
	fluid_sim_t();

	bool init();
	void resize(int w, int h);
	void reset();
	void render();
	void idle();

	void key_up(int key);
	void key_down(int key);

	void mouse_move(int x, int y);
	void mouse_up(int x, int y, int btn);
	void mouse_down(int x, int y, int btn);
	void mouse_drag(int x, int y, int btn);
	void mouse_wheel(int x, int y, int dir);

private:
	int win_width_, win_height_;
	float win_scale_x_, win_scale_y_;

	float main_area_x0_, main_area_y0_;
	float main_area_x1_, main_area_y1_;

	float side_area_x0_, side_area_y0_;
	float side_area_x1_, side_area_y1_;

	float max_force_;
	std::vector<fluid_particle_t*> particles_;

	index_grid_t index_grid_;
	particle_emitter_t emitter_;

	poly6_t poly6_;
	spiky_t spiky_;
	viscosity_t viscosity_;

	bool ready_;
	glm::vec2 end_point_[2], mouse_pos_;

	std::vector<obb_t> draw_obb_, edge_obb_;

	bool obb_locked_;
	glm::vec2 center_, offset_;

	glm::vec2 elastic_start_;
	obb_t elastic_obb_;
	std::unordered_set<int> selected_obb_;

	float rot_speed_;

	bool leak_, show_edge_;

	int force_coeff_;
};

#endif /* FLUID_SIM_INCLUDED */
