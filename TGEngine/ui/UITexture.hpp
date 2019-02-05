#pragma once

#include "UI.hpp"

struct UITexture : UIComponent {

	UITexture(glm::vec4 rect, char* string);
	UITexture(glm::vec4 rect, Texture* tex);

	Texture tex;
	glm::vec4 color = {1, 1, 1, 1};
};

void drawTexture(void* c, IndexBuffer* idx, VertexBuffer* vert);