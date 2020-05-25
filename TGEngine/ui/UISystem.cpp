#include "UISystem.hpp"
#include "../pipeline/window/Window.hpp"
#include <string>
#include "../pipeline/Draw.hpp"

using namespace tge::win;

namespace tge::ui {

	std::vector<BoundingBox> boundingBoxes;
	std::vector<std::function<void(uint32_t)>> boundingBoxFunctions;

	void checkBoundingBoxes() {
		for (uint32_t i = 0; i < boundingBoxes.size(); i++) {
			BoundingBox box = boundingBoxes[i];
			if (box.topLeftX <= mouseHomogeneousX && box.bottomRightX >= mouseHomogeneousX
				&& box.topLeftY <= mouseHomogeneousY && box.bottomRightY >= mouseHomogeneousY) {
				auto func = boundingBoxFunctions[i];
				if (func)
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

	void createTextInput(const TextInputInfo& textInput, std::function<void(std::string)> promis) {
		std::string stringbuffer("_");
		stringbuffer.reserve(100);
		const glm::mat4 position = drw::genMatrix(textInput.x, textInput.y, textInput.z, textInput.scale, textInput.scale);

		uint32_t createdStringId = fnt::createStringActor(textInput.font, &stringbuffer, 1, &position);

		tge::io::keyboardHandler.push_back([exit = false, isShiftPressed = false, stringbuffer = std::move(stringbuffer), position = position, //
			currentHandlerId = (uint32_t)tge::io::keyboardHandler.size(), createdStringId = createdStringId, font=textInput.font, //
			func=std::move(promis)](uint16_t vkey, bool pressed) mutable {
			OUT_LV_DEBUG(vkey << " " << pressed);
			if (exit)
				return;
			// Equals to [a-z0-9]
			if ((vkey > 0x40 && vkey < 0x5B) || (vkey > 0x30 && vkey < 0x39)) {
				if (pressed)
					return;
				stringbuffer[stringbuffer.length() - 1] = (char)vkey + (isShiftPressed * 0x20);
				stringbuffer.push_back('_');
			} else if (vkey == VK_SHIFT) {
				isShiftPressed = !pressed;
				return;
			} else if (vkey == VK_BACK) {
				if (pressed || stringbuffer.length() < 2)
					return;
				stringbuffer[stringbuffer.length() - 2] = '_';
				stringbuffer.pop_back();
			} else if (vkey == VK_RETURN) {
				exit = true;
				fnt::destroyStrings(createdStringId);
				executionQueue.push_back(fillCommandBuffer);
				stringbuffer.pop_back();
				func(stringbuffer);
				tge::io::keyboardHandler.erase(tge::io::keyboardHandler.begin() + currentHandlerId, tge::io::keyboardHandler.begin() + currentHandlerId + 1);
				return;
			}
			fnt::destroyStrings(createdStringId);
			createdStringId = fnt::createStringActor(font, &stringbuffer, 1, &position);
			executionQueue.push_back(fillCommandBuffer);
		});
	}
}