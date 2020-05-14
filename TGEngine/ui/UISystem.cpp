#include "UISystem.hpp"
#include "../pipeline/window/Window.hpp"

using namespace tge::win;

namespace tge::ui {

	std::vector<BoundingBox> boundingBoxes;
	std::vector<std::function<void(uint32_t)>> boundingBoxFunctions;

	void checkBoundingBoxes() {
		for (size_t i = 0; i < boundingBoxes.size(); i++) {
			BoundingBox box = boundingBoxes[i];
			if (box.topLeftX <= mouseHomogeneousX && box.bottomRightX >= mouseHomogeneousX
				&& box.topLeftY <= mouseHomogeneousY && box.bottomRightY >= mouseHomogeneousY) {
				auto func = boundingBoxFunctions[i];
				if(func)
					func(i);
			}
		}
	}

	void deleteBoundingBoxes(uint32_t start, uint32_t end) {
		auto itr = boundingBoxes.begin();
		boundingBoxes.erase(itr + start, itr + end);
		auto itr2 = boundingBoxFunctions.begin();
		boundingBoxFunctions.erase(itr2 + start, itr2 + end);
	}
}