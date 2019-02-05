#include "UIButton.hpp"

UIButton::UIButton(Font* font, glm::vec4 rect, glm::vec4 color, char* string) : UIComponent(rect)
{
	this->draw_call = &drawButton;
	this->color = color;
	this->string = string;
	this->font = font;
	this->font_color = { 1, 1, 1, 1 };
}

void drawButton(void* c, IndexBuffer* idx, VertexBuffer* vert) {
	UIButton* comp = (UIButton*)c;
	comp->font->drawString({
	{comp->rect.x, comp->rect.y, 0.011f},
	comp->font_color
		}, comp->string, vert, idx);
	drawRectangle({
{comp->rect.x, comp->rect.y, 0.01f},
comp->color,
{0, 0},
COLOR_ONLY
		}, comp->rect.z, comp->rect.w, vert, idx);
}
