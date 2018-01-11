#include "Rectangle.hpp"

void drawRectangle(Vertex vert, float xsize, float ysize, VertexBuffer* buffer) {
	buffer->add({
		{ vert.position.x - xsize, vert.position.y - ysize, 0 },
		vert.color
	});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y - ysize, 0 },
		vert.color
	});
	buffer->add({
		{ vert.position.x - xsize, vert.position.y + ysize, 0 },
		vert.color
	});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y - ysize, 0 },
		vert.color
	});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y + ysize, 0 },
		vert.color
	});
	buffer->add({
		{ vert.position.x - xsize, vert.position.y + ysize, 0 },
		vert.color
	});
}