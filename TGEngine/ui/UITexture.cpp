#include "UITexture.hpp"

UITexture::UITexture(glm::vec4 rect, char* string) : UIComponent(rect)
{
	this->tex = {
		string
	};
	createTexture(&this->tex);
	this->draw_call = &drawTexture;
}

UITexture::UITexture(glm::vec4 rect, Texture* tex) : UIComponent(rect)
{
	this->tex = *tex;
	this->draw_call = &drawTexture;
}

void drawTexture(void* c, IndexBuffer* idx, VertexBuffer* vert) {
	UITexture* comp = (UITexture*)c;
	glm::vec4 rect = comp->getRect();
	drawRectangle({
		{rect.x, rect.y, 0},
		comp->color,
		{0, 0},
		comp->tex.index
		}, rect.z, rect.w, vert, idx);
}