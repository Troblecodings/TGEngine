#pragma once

#include <cstdint>

#define PI 3.14159265358979323846
#define PIx2 6.28318530717958647692

namespace math {

	float max(float val1, float val2);

	int max(int val1, int val2);

	float min(float val1, float val2);

	int min(int val1, int val2);

	std::uint32_t max(std::uint32_t val1, std::uint32_t val2);

	std::uint32_t min(std::uint32_t val1, std::uint32_t val2);

}