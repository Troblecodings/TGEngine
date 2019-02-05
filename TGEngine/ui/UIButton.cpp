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
	glm::vec4 rect = comp->getRect();
	drawRectangle({
		   {rect.x, rect.y, 0.012f},
		   comp->color,
		   {0, 0},
		   COLOR_ONLY
		},
	rect.z, rect.w, vert, idx);
	comp->font->drawString({
	    {rect.x, rect.y, 0.011f},
	    comp->font_color
	}, comp->string, vert, idx);
}
