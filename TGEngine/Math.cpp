#include "Math.hpp"

namespace math {

	float max(float val1, float val2) {
		if (val1 > val2) {
			return val1;
		}
		return val2;
	}

	int max(int val1, int val2) {
		if (val1 > val2) {
			return val1;
		}
		return val2;
	}

	float min(float val1, float val2) {
		if (val1 < val2) {
			return val1;
		}
		return val2;
	}

	int min(int val1, int val2) {
		if (val1 < val2) {
			return val1;
		}
		return val2;
	}

	std::uint32_t max(std::uint32_t val1, std::uint32_t val2) {
		if (val1 > val2) {
			return val1;
		}
		return val2;
	}

	std::uint32_t min(std::uint32_t val1, std::uint32_t val2) {
		if (val1 < val2) {
			return val1;
		}
		return val2;
	}

}