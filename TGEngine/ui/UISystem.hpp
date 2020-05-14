#pragma once

#include <vector>
#include <functional>
#include "../io/Font.hpp"
#include "../drawlib/Quad.hpp"

namespace tge::ui {

	struct BoundingBox {
		float topLeftX;
		float topLeftY;
		float bottomRightX;
		float bottomRightY;
	};

	struct ListInputInfo {
		float    scalefactor;
		float    heightOffset;
		float    startX;
		float    width;
		float    startY;
		uint32_t size;
	};

	extern std::vector<BoundingBox> boundingBoxes;
	extern std::vector<std::function<void(uint32_t)>> boundingBoxFunctions;

	void checkBoundingBoxes();

	void deleteBoundingBoxes(uint32_t start, uint32_t end);

	inline void createList(const ListInputInfo* listInputInfo, const float z, const uint32_t size, glm::mat4* output, const bool generate = false, std::function<void(uint32_t)> function = NULL) {
		for (uint32_t i = 0; i < size; i++) {
			ListInputInfo listInput = listInputInfo[i];
			for (uint32_t j = 0; j < listInput.size; j++) {
				output[j] = drw::genMatrix(listInput.startX, listInput.startY, z, listInput.scalefactor, listInput.scalefactor);
				listInput.startY += listInput.heightOffset;
				if (generate) {
					boundingBoxes.push_back({ listInput.startX, listInput.startY, listInput.startX + listInput.width, listInput.startY + (listInput.heightOffset / 2) });
					boundingBoxFunctions.push_back(function);
				}
			}
		}
	}
}