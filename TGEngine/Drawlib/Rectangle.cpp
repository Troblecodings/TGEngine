                                      #include "Rectangle.hpp"

void drawRectangle(Vertex vert, float xsize, float ysize, VertexBuffer* buffer) {
	buffer->add({
		{ vert.position.x - xsize, vert.position.y - ysize, vert.position.z },
		vert.color,
		vert.uv,
		vert.only_color
	});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y - ysize, vert.position.z },
		vert.color,
		vert.uv,
		vert.only_color
	});
	buffer->add({
		{ vert.position.x - xsize, vert.position.y + ysize, vert.position.z },
		vert.color,
		vert.uv,
		vert.only_color
	});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y - ysize, vert.position.z },
		vert.color,
		vert.uv,
		vert.only_color
	});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y + ysize, vert.position.z },
		vert.color,
		vert.uv,
		vert.only_color
	});
	buffer->add({
		{ vert.position.x - xsize, vert.position.y + ysize, vert.position.z },
		vert.color,
		vert.uv,
		vert.only_color
	});
}

void drawRectangleZ(Vertex vert, float xsize, float depth, VertexBuffer* buffer) {
	buffer->add({
		{ vert.position.x - xsize, vert.position.y, vert.position.z - depth},
		vert.color,
		vert.uv,
		vert.only_color
		});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y, vert.position.z - depth},
		vert.color,
		vert.uv,
		vert.only_color
		});
	buffer->add({
		{ vert.position.x - xsize, vert.position.y , vert.position.z + depth },
		vert.color,
		vert.uv,
		vert.only_color
		});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y, vert.position.z - depth},
		vert.color,
		vert.uv,
		vert.only_color
		});
	buffer->add({
		{ vert.position.x + xsize, vert.position.y, vert.position.z + depth},
		vert.color,
		vert.uv,
		vert.only_color
		});
	buffer->add({
		{ vert.position.x - xsize, vert.position.y, vert.position.z + depth},
		vert.color,
		vert.uv,
		vert.only_color
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