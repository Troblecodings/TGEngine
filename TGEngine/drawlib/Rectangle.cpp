#include "Rectangle.hpp"

void drawRectangleZ(TGVertex vert, float xsize, float depth, VertexBuffer* buffer, IndexBuffer* ibuffer) {
	uint32_t idcount = (uint32_t)buffer->count_of_points;
	buffer->add({
		{ vert.position.x - xsize, vert.position.y, vert.position.z - depth },
		vert.color,
		{ 0, 0 },
		vert.color_only
		});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y, vert.position.z - depth },
		vert.color,
		{ 1, 0 },
		vert.color_only
		});
	buffer->add({
	    { vert.position.x + xsize, vert.position.y , vert.position.z + depth },
	    vert.color,
	    { 1, 1 },
	    vert.color_only
		});
	buffer->add({
	    { vert.position.x - xsize, vert.position.y, vert.position.z + depth },
	    vert.color,
	    { 0, 1 },
	    vert.color_only
		});
	ibuffer->addIndex(idcount);
	ibuffer->addIndex(idcount + 1);
	ibuffer->addIndex(idcount + 2);
	ibuffer->addIndex(idcount);
	ibuffer->addIndex(idcount + 2);
	ibuffer->addIndex(idcount + 3);
}

void drawRectangle(glm::vec3 pos, glm::vec4 color, uint32_t color_index, float xsize, float ysize, VertexBuffer* buffer, IndexBuffer* ibuffer) {
	uint32_t idcount = (uint32_t)buffer->count_of_points;
	buffer->add({
		{ pos.x - xsize, pos.y - ysize, pos.z },
		color,
		{ 0, 0 },
		color_index
		});
	buffer->add({
		{ pos.x + xsize, pos.y - ysize, pos.z },
		color,
		{ 1, 0 },
		color_index
		});
	buffer->add({
		{ pos.x + xsize, pos.y + ysize, pos.z },
		color,
		{ 1, 1 },
		color_index
		});
	buffer->add({
		{ pos.x - xsize, pos.y + ysize, pos.z },
		color,
	    { 0, 1 },
		color_index
		});
	ibuffer->addIndex(idcount);
	ibuffer->addIndex(idcount + 1);
	ibuffer->addIndex(idcount + 2);
	ibuffer->addIndex(idcount);
	ibuffer->addIndex(idcount + 2);
	ibuffer->addIndex(idcount + 3);
}