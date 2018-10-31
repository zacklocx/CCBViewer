
#ifndef DRAW_INCLUDED
#define DRAW_INCLUDED

void use_color(int color_index);
void use_color(const char* color_str);

void blend_mode(int src_index, int dst_index);

void draw_line(float x1, float y1, float x2, float y2, float width);
void draw_rect(float x, float y, float width, float height, bool fill);
void draw_circle(float x, float y, float inner_radius, float outer_radius);

#endif /* DRAW_INCLUDED */
