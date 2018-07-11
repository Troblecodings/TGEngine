#include "Circle.hpp"

void drawCircle(Vertex pos, float radius, int max_verticies, VertexBuffer* vert) {
	float angle = PIx2 / max_verticies, l_x, l_y;

	for (size_t i = 0; i < max_verticies + 1; i++)
	{
		float r_angle = angle * i, x = pos.pos[0] +radius * cos(r_angle), y = pos.pos[1] + radius * sin(r_angle);

		if (i != 0) {
		    vert->addColorOnly(pos);
			vert->addColorOnly({
				{l_x, l_y, 0},
				{pos.color[0], pos.color[1], pos.color[2], pos.color[3] }
			});
			vert->addColorOnly({
				{ x, y, 0 },
				{ pos.color[0], pos.color[1], pos.color[2], pos.color[3] }
			});
		}

		l_x = x;
		l_y = y;
	}
}