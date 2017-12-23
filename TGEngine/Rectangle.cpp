#include "Rectangle.hpp"

void drawRectangle(Vertex vert, float xsize, float ysize, std::vector<Vertex>* vertices) {
	uint32_t csize = vertices->size();
	vertices->resize(csize + 6);
	(*vertices)[csize] = {
		{ vert.position.x - xsize, vert.position.y - ysize, 0},
		vert.color
	};
	(*vertices)[csize + 1] = {
		{ vert.position.x + xsize, vert.position.y - ysize, 0 },
		vert.color
	};
	(*vertices)[csize + 2] = {
		{ vert.position.x - xsize, vert.position.y + ysize, 0 },
		vert.color
	};
	(*vertices)[csize + 3] = {
		{ vert.position.x + xsize, vert.position.y - ysize, 0 },
		vert.color
	};
	(*vertices)[csize + 4] = {
		{ vert.position.x + xsize, vert.position.y + ysize, 0 },
		vert.color
	};
	(*vertices)[csize + 5] = {
		{ vert.position.x - xsize, vert.position.y + ysize, 0 },
		vert.color
	};
}