#include "../../public/ui/UISystem.hpp"
#include "../../public/pipeline/window/Window.hpp"
#include <string>
#include "../../public/pipeline/Draw.hpp"

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

	void createTextInput(const TextInputInfo& textInput, std::function<void(std::string)> exitFunction) {
		std::string stringBuffer("_");
		stringBuffer.reserve(100);
		const glm::mat4 position = drw::genMatrix(textInput.x, textInput.y, textInput.z, textInput.scale, textInput.scale);

		uint32_t createdStringId = fnt::createStringActor(textInput.font, &stringBuffer, 1, &position);

		tge::io::keyboardHandler.push_back([exit = false, isShiftNotPressed = true, stringBuffer = std::move(stringBuffer), position = position, //
			currentHandlerId = (uint32_t)tge::io::keyboardHandler.size(), createdStringId = createdStringId, font=textInput.font, //
			func=std::move(exitFunction)](uint16_t vkey, bool pressed) mutable {
			if (exit)
				return;
			// Equals to [a-z0-9]
			if ((vkey > 0x40 && vkey < 0x5B) || (vkey > 0x30 && vkey < 0x39)) {
				if (pressed)
					return;
				stringBuffer[stringBuffer.length() - 1] = (char)vkey + (isShiftNotPressed * 0x20);
				stringBuffer.push_back('_');
			} else if (vkey == VK_SHIFT) {
				isShiftNotPressed = pressed;
				return;
			} else if (vkey == VK_BACK) {
				if (pressed || stringBuffer.length() < 2)
					return;
				stringBuffer[stringBuffer.length() - 2] = '_';
				stringBuffer.pop_back();
			} else if (vkey == VK_RETURN) {
				exit = true;
				fnt::destroyStrings(createdStringId);
				executionQueue.push_back(fillCommandBuffer);
				stringBuffer.pop_back();
				func(stringBuffer);
				tge::io::keyboardHandler.erase(tge::io::keyboardHandler.begin() + currentHandlerId, tge::io::keyboardHandler.begin() + currentHandlerId + 1);
				return;
			}
			fnt::destroyStrings(createdStringId);
			createdStringId = fnt::createStringActor(font, &stringBuffer, 1, &position);
			executionQueue.push_back(fillCommandBuffer);
		});
	}
}