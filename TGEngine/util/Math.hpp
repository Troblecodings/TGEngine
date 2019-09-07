#pragma once

#include "Annotations.hpp"
#include <cstdint>

#define PI 3.14159265358979323846
#define PIx2 6.28318530717958647692

#define TGE_MIN(a,b) (a < b ? a:b)
#define TGE_MAX(a,b) (a > b ? a:b)

namespace math {

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