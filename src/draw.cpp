
#include "draw.h"

#include <cstring>

#include <GL/gl.h>
#include <Gl/glu.h>

static const char* color_lib[][2] =
{
	{ "aliceblue", "#F0F8FF" },
	{ "antiquewhite", "#FAEBD7" },
	{ "aqua", "#00FFFF" },
	{ "aquamarine", "#7FFFD4" },
	{ "azure", "#F0FFFF" },
	{ "beige", "#F5F5DC" },
	{ "bisque", "#FFE4C4" },
	{ "black", "#000000" },
	{ "blanchedalmond", "#FFEBCD" },
	{ "blue", "#0000FF" },
	{ "blueviolet", "#8A2BE2" },
	{ "brown", "#A52A2A" },
	{ "burlywood", "#DEB887" },
	{ "cadetblue", "#5F9EA0" },
	{ "chartreuse", "#7FFF00" },
	{ "chocolate", "#D2691E" },
	{ "coral", "#FF7F50" },
	{ "cornflowerblue", "#6495ED" },
	{ "cornsilk", "#FFF8DC" },
	{ "crimson", "#DC143C" },
	{ "cyan", "#00FFFF" },
	{ "darkblue", "#00008B" },
	{ "darkcyan", "#008B8B" },
	{ "darkgoldenrod", "#B8860B" },
	{ "darkgray", "#A9A9A9" },
	{ "darkgreen", "#006400" },
	{ "darkkhaki", "#BDB76B" },
	{ "darkmagenta", "#8B008B" },
	{ "darkolivegreen", "#556B2F" },
	{ "darkorange", "#FF8C00" },
	{ "darkorchid", "#9932CC" },
	{ "darkred", "#8B0000" },
	{ "darksalmon", "#E9967A" },
	{ "darkseagreen", "#8FBC8B" },
	{ "darkslateblue", "#483D8B" },
	{ "darkslategray", "#2F4F4F" },
	{ "darkturquoise", "#00CED1" },
	{ "darkviolet", "#9400D3" },
	{ "deeppink", "#FF1493" },
	{ "deepskyblue", "#00BFFF" },
	{ "dimgray", "#696969" },
	{ "dodgerblue", "#1E90FF" },
	{ "firebrick", "#B22222" },
	{ "floralwhite", "#FFFAF0" },
	{ "forestgreen", "#228B22" },
	{ "fuchsia", "#FF00FF" },
	{ "gainsboro", "#DCDCDC" },
	{ "ghostwhite", "#F8F8FF" },
	{ "gold", "#FFD700" },
	{ "goldenrod", "#DAA520" },
	{ "gray", "#808080" },
	{ "green", "#008000" },
	{ "greenyellow", "#ADFF2F" },
	{ "honeydew", "#F0FFF0" },
	{ "hotpink", "#FF69B4" },
	{ "indianred", "#CD5C5C" },
	{ "indigo", "#4B0082" },
	{ "ivory", "#FFFFF0" },
	{ "khaki", "#F0E68C" },
	{ "lavender", "#E6E6FA" },
	{ "lavenderblush", "#FFF0F5" },
	{ "lawngreen", "#7CFC00" },
	{ "lemonchiffon", "#FFFACD" },
	{ "lightblue", "#ADD8E6" },
	{ "lightcoral", "#F08080" },
	{ "lightcyan", "#E0FFFF" },
	{ "lightgoldenrodyellow", "#FAFAD2" },
	{ "lightgreen", "#90EE90" },
	{ "lightgrey", "#D3D3D3" },
	{ "lightpink", "#FFB6C1" },
	{ "lightsalmon", "#FFA07A" },
	{ "lightseagreen", "#20B2AA" },
	{ "lightskyblue", "#87CEFA" },
	{ "lightslategray", "#778899" },
	{ "lightsteelblue", "#B0C4DE" },
	{ "lightyellow", "#FFFFE0" },
	{ "lime", "#00FF00" },
	{ "limegreen", "#32CD32" },
	{ "linen", "#FAF0E6" },
	{ "magenta", "#FF00FF" },
	{ "maroon", "#800000" },
	{ "mediumaquamarine", "#66CDAA" },
	{ "mediumblue", "#0000CD" },
	{ "mediumorchid", "#BA55D3" },
	{ "mediumpurple", "#9370DB" },
	{ "mediumseagreen", "#3CB371" },
	{ "mediumslateblue", "#7B68EE" },
	{ "mediumspringgreen", "#00FA9A" },
	{ "mediumturquoise", "#48D1CC" },
	{ "mediumvioletred", "#C71585" },
	{ "midnightblue", "#191970" },
	{ "mintcream", "#F5FFFA" },
	{ "mistyrose", "#FFE4E1" },
	{ "moccasin", "#FFE4B5" },
	{ "navajowhite", "#FFDEAD" },
	{ "navy", "#000080" },
	{ "oldlace", "#FDF5E6" },
	{ "olive", "#808000" },
	{ "olivedrab", "#6B8E23" },
	{ "orange", "#FFA500" },
	{ "orangered", "#FF4500" },
	{ "orchid", "#DA70D6" },
	{ "palegoldenrod", "#EEE8AA" },
	{ "palegreen", "#98FB98" },
	{ "paleturquoise", "#AFEEEE" },
	{ "palevioletred", "#DB7093" },
	{ "papayawhip", "#FFEFD5" },
	{ "peachpuff", "#FFDAB9" },
	{ "peru", "#CD853F" },
	{ "pink", "#FFC0CB" },
	{ "plum", "#DDA0DD" },
	{ "powderblue", "#B0E0E6" },
	{ "purple", "#800080" },
	{ "red", "#FF0000" },
	{ "rosybrown", "#BC8F8F" },
	{ "royalblue", "#4169E1" },
	{ "saddlebrown", "#8B4513" },
	{ "salmon", "#FA8072" },
	{ "sandybrown", "#F4A460" },
	{ "seagreen", "#2E8B57" },
	{ "seashell", "#FFF5EE" },
	{ "sienna", "#A0522D" },
	{ "silver", "#C0C0C0" },
	{ "skyblue", "#87CEEB" },
	{ "slateblue", "#6A5ACD" },
	{ "slategray", "#708090" },
	{ "snow", "#FFFAFA" },
	{ "springgreen", "#00FF7F" },
	{ "steelblue", "#4682B4" },
	{ "tan", "#D2B48C" },
	{ "teal", "#008080" },
	{ "thistle", "#D8BFD8" },
	{ "tomato", "#FF6347" },
	{ "turquoise", "#40E0D0" },
	{ "violet", "#EE82EE" },
	{ "wheat", "#F5DEB3" },
	{ "white", "#FFFFFF" },
	{ "whitesmoke", "#F5F5F5" },
	{ "yellow", "#FFFF00" },
	{ "yellowgreen", "#9ACD32" }
};

static const int default_color_index = 125;

static void hex_to_rgb(const char* hex, unsigned char* rgb)
{
	if(!hex || !rgb)
	{
		return;
	}

	char c;
	int tmp[6];

	for(int i = 0; i < 6; ++i)
	{
		c = hex[i + 1];

		if(c >= '0' && c <= '9')
		{
			tmp[i] = c - '0';
		}
		else
		{
			tmp[i] = (c - 'A') + 10;
		}

		if(i & 1)
		{
			rgb[i >> 1] = (tmp[i - 1] << 4) + tmp[i];
		}
	}
}

void use_color(const char* color_str)
{
	if(!color_str)
	{
		return;
	}

	const char* hex = nullptr;
	unsigned char rgb[3];

	if(color_str[0] != '#')
	{
		for(const auto& it : color_lib)
		{
			if(0 == strcmp(color_str, it[0]))
			{
				hex = it[1];
				break;
			}
		}

		if(!hex)
		{
			hex = color_lib[default_color_index][1];
		}
	}
	else
	{
		hex = color_str;
	}

	hex_to_rgb(hex, rgb);

	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3ubv(rgb);
}

static const GLenum blend_factor[] =
{
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR
};

static const int blend_factor_num = sizeof(blend_factor) / sizeof(blend_factor[0]);

void blend_mode(int src_index, int dst_index)
{
	if(src_index >= 0 && src_index <= blend_factor_num - 1 &&
		dst_index >= 0 && dst_index <= blend_factor_num - 1)
	{
		glEnable(GL_BLEND);
		glBlendFunc(blend_factor[src_index], blend_factor[dst_index]);
	}
	else
	{
		glDisable(GL_BLEND);
	}
}

void draw_line(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();
}

void draw_rect(float x, float y, float width, float height, bool fill)
{
	int mode[] = { GL_LINE_LOOP, GL_QUADS };

	float vx1 = x - width * 0.5f, vy1 = y - height * 0.5f;
	float vx2 = x + width * 0.5f, vy2 = y + height * 0.5f;

	glBegin(mode[fill]);
		glVertex2f(vx1, vy1);
		glVertex2f(vx2, vy1);
		glVertex2f(vx2, vy2);
		glVertex2f(vx1, vy2);
	glEnd();
}

void draw_circle(float x, float y, float inner_radius, float outer_radius)
{
	static GLUquadricObj* q = gluNewQuadric();

	glPushMatrix();
	glTranslatef(x, y, 0.0f);

	gluPartialDisk(q, inner_radius, outer_radius, 32, 32, 0.0, 360.0);

	glPopMatrix();
}
