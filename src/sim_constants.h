
#ifndef SIM_CONSTANTS_INCLUDED
#define SIM_CONSTANTS_INCLUDED

#include <glm/glm.hpp>

const glm::vec2 gravity(0.0f, -9.8f);

const float density_offset = 1200.0f;
const float gas_k = 0.2f;
const float visco_coeff = 0.002f;

const float sim_left = 0.0f;
const float sim_right = 12.0f;
const float sim_bottom = 0.0f;
const float sim_top = 9.0f;

const float sim_width = sim_right - sim_left;
const float sim_height = sim_top - sim_bottom;

const float cell_space = sim_width / 64.0f;

const float particle_mass = cell_space * 25.0f;

const float delta_time_sec = 0.012f;

#endif
