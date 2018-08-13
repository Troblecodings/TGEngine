#include "Rectangle.hpp"

void drawRectangle(TGVertex vert, float xsize, float ysize, VertexBuffer* buffer) {
	buffer->add({
		{ vert.position.x - xsize, vert.position.y - ysize, vert.position.z },
		vert.color,
		vert.uv,
		vert.color_only
		});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y - ysize, vert.position.z },
		vert.color,
		vert.uv,
		vert.color_only
		});
	buffer->add({
		{ vert.position.x - xsize, vert.position.y + ysize, vert.position.z },
		vert.color,
		vert.uv,
		vert.color_only
		});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y - ysize, vert.position.z },
		vert.color,
		vert.uv,
		vert.color_only
		});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y + ysize, vert.position.z },
		vert.color,
		vert.uv,
		vert.color_only
		});
	buffer->add({
		{ vert.position.x - xsize, vert.position.y + ysize, vert.position.z },
		vert.color,
		vert.uv,
		vert.color_only
		});
}

void drawRectangleZ(TGVertex vert, float xsize, float depth, VertexBuffer* buffer) {
	buffer->add({
		{ vert.position.x - xsize, vert.position.y, vert.position.z - depth },
		vert.color,
		vert.uv,
		vert.color_only
		});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y, vert.position.z - depth },
		vert.color,
		vert.uv,
		vert.color_only
		});
	buffer->add({
		{ vert.position.x - xsize, vert.position.y , vert.position.z + depth },
		vert.color,
		vert.uv,
		vert.color_only
		});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y, vert.position.z - depth },
		vert.color,
		vert.uv,
		vert.color_only
		});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y, vert.position.z + depth },
		vert.color,
		vert.uv,
		vert.color_only
		});
	buffer->add({
		{ vert.position.x - xsize, vert.position.y, vert.position.z + depth },
		vert.color,
		vert.uv,
		vert.color_only
		});
}

void drawRectangleWithTexture(TGVertex vert, float xsize, float ysize, VertexBuffer* buffer) {
	buffer->add({
		{ vert.position.x - xsize, vert.position.y - ysize, vert.position.z },
		vert.color,
		{ 0, 0 },
		vert.color_only
		});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y - ysize, vert.position.z },
		vert.color,
		{ 1, 0 },
		vert.color_only
		});
	buffer->add({
		{ vert.position.x - xsize, vert.position.y + ysize, vert.position.z },
		vert.color,
		{ 0, 1 },
		vert.color_only
		});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y - ysize, vert.position.z },
		vert.color,
		{ 1, 0 },
		vert.color_only
		});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y + ysize, vert.position.z },
		vert.color,
		{ 1, 1 },
		vert.color_only
		});
	buffer->add({
		{ vert.position.x - xsize, vert.position.y + ysize, vert.position.z },
		vert.color,
		{ 0, 1 },
		vert.color_only
		});
}