#include "Ellipse.hpp"

void drawEllipse(TGVertex pos, float xradius, float yradius, int max_verticies, VertexBuffer* vert, IndexBuffer* ibuffer) {
	double angle = PIx2 / max_verticies, l_x, l_y;

	for (size_t i = 0; i < max_verticies + 1; i++)
	{
		double r_angle = angle * i, x = pos.position.x + xradius * cos(r_angle), y = pos.position.y + yradius * sin(r_angle);

		if (i != 0) {
			vert->addColorOnly(pos);
			vert->addColorOnly({
				{ l_x, l_y, 0 },
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