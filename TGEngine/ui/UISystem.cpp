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
				boundingBoxFunctions[i](i);
			}
		}
	}

}