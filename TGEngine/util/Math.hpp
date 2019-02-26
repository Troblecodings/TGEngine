#pragma once

#include "Annotations.hpp"
#include <cstdint>
#include <cmath>
#include "glm/glm.hpp"

#define PI 3.14159265358979323846
#define PIx2 6.28318530717958647692
#define SQRT_PIx2 2.506628274631
#define EULER 2.71828182845904523536

namespace math {

	SINCE(0, 0, 4)
	glm::vec4 mixColor(glm::vec4 a, glm::vec4 b, float t = 0.5f);

	SINCE(0, 0, 4)
	float calculateNormalDistribution(float x, float mu = 0.0f, float sigma = 1.0f);

	SINCE(0, 0, 1)
	float f_max(float val1, float val2);

	SINCE(0, 0, 1)
	int i_max(int val1, int val2);

	SINCE(0, 0, 1)
	float f_min(float val1, float val2);

	SINCE(0, 0, 1)
	int i_min(int val1, int val2);

	SINCE(0, 0, 1)
	std::uint32_t u_max(std::uint32_t val1, std::uint32_t val2);

	SINCE(0, 0, 1)
	std::uint32_t u_min(std::uint32_t val1, std::uint32_t val2);

}