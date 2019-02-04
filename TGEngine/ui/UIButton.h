#pragma once

#include "UI.h"

struct UIButton : UIComponent
{
	UIButton(Font* font, glm::vec4 rect, glm::vec4 color, char* string);

	glm::vec4 rect;
	glm::vec4 color;
	char* string;
	Font* font;
};

void drawButton(void* comp, IndexBuffer* idx, VertexBuffer* vert);
