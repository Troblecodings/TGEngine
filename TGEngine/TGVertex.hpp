#pragma once

#include "glm/glm.hpp"

struct TGVertex {
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 uv;
	uint32_t color_only;
};

#define VERTEX_SIZE sizeof(TGVertex)
