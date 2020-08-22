#pragma once

#include "../../public/util/glm/glm.hpp"
#include "../../public/util/VectorUtil.hpp"

struct TGVertex {
	glm::vec3 position;
	glm::vec2 uv = { 0, 0 };
	glm::vec3 normal = glm::vec3(1.0f, 1.0f, 1.0f);

	bool operator==(const TGVertex& lhs);
};

#define VERTEX_SIZE sizeof(TGVertex)
