#pragma once

#include "glm/glm.hpp"
#include "VectorUtil.hpp"

struct Renderer {
	std::vector<TGVertex>* vbuffer;
	std::vector<uint32_t>* ibuffer;
};

struct TGVertex {
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 uv;
	uint32_t color_only;
	glm::vec3 normal = glm::vec3(1.0f, 1.0f, 1.0f);

	bool operator==(const TGVertex& lhs);
	void operator>>(Renderer* render);
};

#define VERTEX_SIZE sizeof(TGVertex)
