#include "Math.hpp"

namespace math {

	float f_max(float val1, float val2) {
		if(val1 > val2) {
			return val1;
		}
		return val2;
	}

	int i_max(int val1, int val2) {
		if(val1 > val2) {
			return val1;
		}
		return val2;
	}

	float f_min(float val1, float val2) {
		if(val1 < val2) {
			return val1;
		}
		return val2;
	}

	int i_min(int val1, int val2) {
		if(val1 < val2) {
			return val1;
		}
		return val2;
	}

	std::uint32_t u_max(std::uint32_t val1, std::uint32_t val2) {
		if(val1 > val2) {
			return val1;
		}
		return val2;
	}

	std::uint32_t u_min(std::uint32_t val1, std::uint32_t val2) {
		if(val1 < val2) {
			return val1;
		}
		return val2;
	}

}