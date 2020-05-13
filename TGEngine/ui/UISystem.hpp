#pragma once

#include <vector>
#include <functional>
#include "../io/Font.hpp"

namespace tge::ui {

	struct BoundingBox {
		float topLeftX;
		float topLeftY;
		float bottomRightX;
		float bottomRightY;
	};

	extern std::vector<BoundingBox> boundingBoxes;
	extern std::vector<std::function<void(uint32_t)>> boundingBoxFunctions;

	void checkBoundingBoxes();

	template<class T = char*>
	struct ListInputInfo {
		const tge::fnt::Font* fontobj;
		const T* stringinput;
		const uint32_t size;
		const float scalefactor;
		const float heightOffset;
	};

	template<class T = char*>
	void createList(ListInputInfo<T>* listInputInfo, uint32_t size, glm::mat4* output) {
		static_assert(!(std::is_same_v<T, std::string> || std::is_same_v<T, char*>), "Only Strings or C-Strings are allowed!");


	}
}