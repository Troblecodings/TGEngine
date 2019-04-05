#include "Rectangle.hpp"

void drawRectangleZ(glm::vec3 pos, float xsize, float zsize, VertexBuffer* buffer, IndexBuffer* ibuffer) {
	uint32_t idcount = (uint32_t)buffer->count_of_points;
	buffer->add({
		{ pos.x - xsize, pos.y, pos.z - zsize },
		{ 0, 0 }
		});
	buffer->add({
		{ pos.x + xsize, pos.y, pos.z - zsize },
		{ 1, 0 }
		});
	buffer->add({
	    { pos.x + xsize, pos.y , pos.z + zsize },
	    { 1, 1 }
		});
	buffer->add({
	    { pos.x - xsize, pos.y, pos.z + zsize },
	    { 0, 1 }
		});
	ibuffer->addIndex(idcount);
	ibuffer->addIndex(idcount + 1);
	ibuffer->addIndex(idcount + 2);
	ibuffer->addIndex(idcount);
	ibuffer->addIndex(idcount + 2);
	ibuffer->addIndex(idcount + 3);
}

void drawRectangle(glm::vec3 pos, float xsize, float ysize, VertexBuffer* buffer, IndexBuffer* ibuffer) {
	uint32_t idcount = (uint32_t)buffer->count_of_points;
	buffer->add({
		{ pos.x, pos.y, pos.z },
		{ 0, 0 }
		});
	buffer->add({
		{ pos.x + xsize, pos.y, pos.z },
		{ 1, 0 }
		});
	buffer->add({
		{ pos.x + xsize, pos.y + ysize, pos.z },
		{ 1, 1 }
		});
	buffer->add({
		{ pos.x, pos.y + ysize, pos.z },
	    { 0, 1 }
		});
	ibuffer->addIndex(idcount);
	ibuffer->addIndex(idcount + 1);
	ibuffer->addIndex(idcount + 2);
	ibuffer->addIndex(idcount);
	ibuffer->addIndex(idcount + 2);
	ibuffer->addIndex(idcount + 3);
}