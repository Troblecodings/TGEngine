#pragma once

#include "UI.hpp"

struct UIButton : UIEntity::UIComponent
{
	UIButton(Font* font, glm::vec4 rect, glm::vec4 color, char* string);

	glm::vec4 color;
	glm::vec4 font_color;
	char* string;
	Font* font;
};