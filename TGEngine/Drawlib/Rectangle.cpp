#include "Rectangle.hpp"

void drawRectangle(Vertex vert, float xsize, float ysize, VertexBuffer* buffer) {
	buffer->addColorOnly({
		{ vert.position.x - xsize, vert.position.y - ysize, 0 },
		vert.color
	});
	buffer->addColorOnly({
		{ vert.position.x + xsize, vert.position.y - ysize, 0 },
		vert.color
	});
	buffer->addColorOnly({
		{ vert.position.x - xsize, vert.position.y + ysize, 0 },
		vert.color
	});
	buffer->addColorOnly({
		{ vert.position.x + xsize, vert.position.y - ysize, 0 },
		vert.color
	});
	buffer->addColorOnly({
		{ vert.position.x + xsize, vert.position.y + ysize, 0 },
		vert.color
	});
	buffer->addColorOnly({
		{ vert.position.x - xsize, vert.position.y + ysize, 0 },
		vert.color
	});
}

void drawRectangleWithTexture(Vertex vert, float xsize, float ysize, VertexBuffer* buffer) {
	buffer->add({
		{ vert.position.x - xsize, vert.position.y - ysize, 0 },
		vert.color,
		{0, 0}
	});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y - ysize, 0 },
		vert.color,
		{1, 0}
	});
	buffer->add({
		{ vert.position.x - xsize, vert.position.y + ysize, 0 },
		vert.color,
		{0, 1}
	});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y - ysize, 0 },
		vert.color,
		{1, 0}
	});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y + ysize, 0 },
		vert.color,
		{1, 1}
	});
	buffer->add({
		{ vert.position.x - xsize, vert.position.y + ysize, 0 },
		vert.color,
		{0, 1}
	});
}