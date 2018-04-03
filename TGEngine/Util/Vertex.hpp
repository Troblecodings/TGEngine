#pragma once

#include "glm\glm.hpp"

struct Vertex {
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 uv;
};

struct Color {
	uint32_t r;
	uint32_t g;
	uint32_t b;
};