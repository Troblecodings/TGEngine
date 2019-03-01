#include "Math.hpp"

namespace math {

	TGVertex midpoint(TGVertex a, TGVertex b, float t)
	{
		return {
			b.position + (a.position - b.position) * t,
			math::mixColor(a.color, b.color, t),
			b.uv + (a.ub - b.uv) * t,
			COLOR_ONLY
		};
	}

	glm::vec4 mixColor(glm::vec4 a, glm::vec4 b, float t)
	{
		float it = 1 - t; // Inverse time
		return glm::vec4(
			sqrt(a.x * a.x * t + b.x * b.x * it),
			sqrt(a.y * a.y * t + b.y * b.y * it),
			sqrt(a.z * a.z * t + b.z * b.z * it),
			sqrt(a.w * a.w * t + b.w * b.w * it)
		);
	}

	float calculateNormalDistribution(float x, float mu, float sigma) {
		return (1 / (sigma * SQRT_PIx2)) * pow(EULER, pow(x - mu, 2) / (2 * sigma * sigma));
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