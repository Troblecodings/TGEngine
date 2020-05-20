#include "UISystem.hpp"
#include "../pipeline/window/Window.hpp"
#include <string>

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
		auto bbIterator = boundingBoxes.begin();
		boundingBoxes.erase(bbIterator + start, bbIterator + end);
		auto bbFunctionIterator = boundingBoxFunctions.begin();
		boundingBoxFunctions.erase(bbFunctionIterator + start, bbFunctionIterator + end);
	}

	struct TextInput {
		float      x;
		float      y;
		float      z;
		float      scaleX;
		float      scaleY;
		fnt::Font* font;
	};

	void createTextInput(const TextInput& textInput, std::function<void(uint32_t)> onExit) {
		std::thread textInputThread([=] {
			std::string stringbuffer("_");
			const glm::mat4 position = drw::genMatrix(textInput.x, textInput.y, textInput.z, textInput.scaleX, textInput.scaleY);

			uint32_t createdStringId = fnt::createStringActor(textInput.font, &stringbuffer, 1, &position);

			const uint32_t currentHandlerId = (uint32_t)tge::io::keyboardHandler.size();
			auto lambda = [&stringbuffer, &createdStringId](uint16_t vkey, bool pressed) mutable {
				OUT_LV_DEBUG(vkey);
				if (vkey <= 128 && pressed) {
					stringbuffer.push_back((char)vkey);
				}
			};
			tge::io::keyboardHandler.push_back(lambda);
		});
		textInputThread.detach();
	}
}