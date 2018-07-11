#include "Rectangle.hpp"

void drawRectangle(Vertex vert, float xsize, float ysize, VertexBuffer* buffer) {
	buffer->add({
		{ vert.pos[0] - xsize, vert.pos[1] - ysize, vert.pos[2] },
		{ vert.color[0], vert.color[1], vert.color[2], vert.color[3] },
		{ 0, 0 },
		vert.color_only
	});
	buffer->add({
		{ vert.pos[0] + xsize, vert.pos[1] - ysize, vert.pos[2] },
		{ vert.color[0], vert.color[1], vert.color[2], vert.color[3] },
		{ 0, 0},
		vert.color_only
	});
	buffer->add({
		{ vert.pos[0] - xsize, vert.pos[1] + ysize, vert.pos[2] },
		{ vert.color[0], vert.color[1], vert.color[2], vert.color[3] },
		{ 0, 0 },
		vert.color_only
	});
	buffer->add({
		{ vert.pos[0] + xsize, vert.pos[1] - ysize, vert.pos[2] },
		{ vert.color[0], vert.color[1], vert.color[2], vert.color[3] },
		{ 0, 0 },
		vert.color_only
	});
	buffer->add({
		{ vert.pos[0] + xsize, vert.pos[1] + ysize, vert.pos[2] },
		{ vert.color[0], vert.color[1], vert.color[2], vert.color[3] },
		{ 0, 0 },
		vert.color_only
	});
	buffer->add({
		{ vert.pos[0] - xsize, vert.pos[1] + ysize, vert.pos[2] },
		{ vert.color[0], vert.color[1], vert.color[2], vert.color[3] },
		{ 0, 0 },
		vert.color_only
	});
}

void drawRectangleZ(Vertex vert, float xsize, float depth, VertexBuffer* buffer) {
	buffer->add({
		{ vert.pos[0] - xsize, vert.pos[1], vert.pos[2] - depth},
		{ vert.color[0], vert.color[1], vert.color[2], vert.color[3] },
		{ 0, 0 },
		vert.color_only
		});
	buffer->add({
		{ vert.pos[0] + xsize, vert.pos[1], vert.pos[2] - depth},
		{ vert.color[0], vert.color[1], vert.color[2], vert.color[3] },
		{ 0, 0 },
		vert.color_only
		});
	buffer->add({
		{ vert.pos[0] - xsize, vert.pos[1] , vert.pos[2] + depth },
		{ vert.color[0], vert.color[1], vert.color[2], vert.color[3] },
		{ 0, 0 },
		vert.color_only
		});
	buffer->add({
		{ vert.pos[0] + xsize, vert.pos[1], vert.pos[2] - depth},
		{ vert.color[0], vert.color[1], vert.color[2], vert.color[3] },
		{ 0, 0 },
		vert.color_only
		});
	buffer->add({
		{ vert.pos[0] + xsize, vert.pos[1], vert.pos[2] + depth},
		{ vert.color[0], vert.color[1], vert.color[2], vert.color[3] },
		{ 0, 0 },
		vert.color_only
		});
	buffer->add({
		{ vert.pos[0] - xsize, vert.pos[1], vert.pos[2] + depth},
		{ vert.color[0], vert.color[1], vert.color[2], vert.color[3] },
		{ 0, 0 },
		vert.color_only
		});
}

void drawRectangleWithTexture(Vertex vert, float xsize, float ysize, VertexBuffer* buffer) {
	buffer->add({
		{ vert.pos[0] - xsize, vert.pos[1] - ysize, vert.pos[2] },
		{ vert.color[0], vert.color[1], vert.color[2], vert.color[3] },
		{0, 0}
	});
	buffer->add({
		{ vert.pos[0] + xsize, vert.pos[1] - ysize, vert.pos[2] },
		{ vert.color[0], vert.color[1], vert.color[2], vert.color[3] },
		{1, 0}
	});
	buffer->add({
		{ vert.pos[0] - xsize, vert.pos[1] + ysize, vert.pos[2] },
		{ vert.color[0], vert.color[1], vert.color[2], vert.color[3] },
		{0, 1}
	});
	buffer->add({
		{ vert.pos[0] + xsize, vert.pos[1] - ysize, vert.pos[2] },
		{ vert.color[0], vert.color[1], vert.color[2], vert.color[3] },
		{1, 0}
	});
	buffer->add({
		{ vert.pos[0] + xsize, vert.pos[1] + ysize, vert.pos[2] },
		{ vert.color[0], vert.color[1], vert.color[2], vert.color[3] },
		{1, 1}
	});
	buffer->add({
		{ vert.pos[0] - xsize, vert.pos[1] + ysize, vert.pos[2] },
		{ vert.color[0], vert.color[1], vert.color[2], vert.color[3] },
		{0, 1}
	});
}