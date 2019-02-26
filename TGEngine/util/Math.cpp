#include "Math.hpp"

namespace math {

	glm::vec4 mixColor(glm::vec4 a, glm::vec4 b, float t = 0.5f)
	{
		float it = 1 - t; // Inverse time
		return glm::vec4(
			sqrtf32(a.x * a.x * t + b.x * b.x * it),
			sqrtf32(a.y * a.y * t + b.y * b.y * it),
			sqrtf32(a.z * a.z * t + b.z * b.z * it),
			sqrtf32(a.w * a.w * t + b.w * b.w * it)
		);
	}

	float calculateNormalDistribution(float x, float mu = 0.0f, float sigma = 1.0f) {
		return (1 / (sigma * SQRT_PIx2)) * powf32(EULER, powf32(x - mu, 2) / (2 * sigma * sigma));
	}

	float f_max(float val1, float val2) {
		if (val1 > val2) {
			return val1;
		}
		return val2;
	}

	int i_max(int val1, int val2) {
		if (val1 > val2) {
			return val1;
		}
		return val2;
	}

	float f_min(float val1, float val2) {
		if (val1 < val2) {
			return val1;
		}
		return val2;
	}

	int i_min(int val1, int val2) {
		if (val1 < val2) {
			return val1;
		}
		return val2;
	}

	std::uint32_t u_max(std::uint32_t val1, std::uint32_t val2) {
		if (val1 > val2) {
			return val1;
		}
		return val2;
	}

	std::uint32_t u_min(std::uint32_t val1, std::uint32_t val2) {
		if (val1 < val2) {
			return val1;
		}
		return val2;
	}

}