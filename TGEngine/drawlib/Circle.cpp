#include "Circle.hpp"

void drawCircle(TGVertex pos, float radius, size_t max_verticies, VertexBuffer* vert, IndexBuffer* ibuffer) {
	double angle = PIx2 / max_verticies, l_x, l_y;

	for (size_t i = 0; i < max_verticies + 1; i++)
	{
		double r_angle = angle * i, x = pos.position.x + radius * cos(r_angle), y = pos.position.y + radius * sin(r_angle);

		if (i != 0) {
		    vert->addColorOnly(pos);
			vert->addColorOnly({
				{l_x, l_y, 0},
				pos.color
			});
			vert->addColorOnly({
				{ x, y, 0 },
				pos.color
			});
		}

		l_x = x;
		l_y = y;
	}
}