#include "Circle.hpp"

void drawCircle(Vertex pos, float radius, int max_verticies, std::vector<Vertex>* vert) {
	float angle = PIx2 / max_verticies;
	uint32_t siz = vert->size();
	vert->resize(siz + max_verticies*2);
	for (size_t i = 0; i < max_verticies; i++)
	{
		float x = pos.position.x + radius * cos(angle*i);
		float y = pos.position.y + radius * sin(angle*i);
		(*vert)[siz + (i * 2) + 1] = {
			{
				x,y,1
            },
			pos.color
		};
		(*vert)[siz + (i * 2)] = pos;
	}
}