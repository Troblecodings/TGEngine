#pragma once

#include "UI.hpp"

struct UIButton : UIComponent
{
	UIButton(Font* font, glm::vec4 rect, glm::vec4 color, char* string);

	glm::vec4 color;
	glm::vec4 font_color;
	char* string;
	Font* font;
};

void drawButton(void* comp, IndexBuffer* idx, VertexBuffer* vert);
