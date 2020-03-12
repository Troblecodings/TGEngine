#pragma once

#include <vector>

namespace tge::ui {
	struct BoundingBox {
		float topLeftX;
		float topLeftY;
		float bottomRightX;
		float bottomRightY;
	};

	extern std::vector<BoundingBox> boundingBoxes;
	extern std::vector<void(*)(void)> boundingBoxFunctions;

	void checkBoundingBoxes();
}