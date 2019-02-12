#pragma once

#include "UI.hpp"

struct UITexture : UIEntity::UIComponent {

	UITexture(glm::vec4 rect, char* string);
	UITexture(glm::vec4 rect, Texture* tex);

	Texture tex;
	glm::vec4 color = {1, 1, 1, 1};
};
