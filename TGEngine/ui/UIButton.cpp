#include "UIButton.h"

UIButton::UIButton(Font* font, glm::vec4 rect, glm::vec4 color, char* string)
{
	this->draw_call = &drawButton;
	this->color = color;
	this->string = string;
	this->rect = rect;
	this->font = font;
}

void drawButton(void* c, IndexBuffer* idx, VertexBuffer* vert) {
	UIButton* comp = (UIButton*)c;
	drawRectangle({
		{comp->rect.x, comp->rect.y, 0},
		comp->color,
	    {0, 0},
		COLOR_ONLY
		}, comp->rect.z, comp->rect.w, vert, idx);
	comp->font->drawString({
		{comp->rect.x, comp->rect.y, 0},
		{1, 1, 1, 1}
		}, comp->string, vert, idx, 1);
}
