#include "UIButton.hpp"

UIButton::UIButton(Font* font, glm::vec4 rect, glm::vec4 color, char* string)
{
	this->color = color;
	this->string = string;
	this->font = font;
	this->font_color = { 1, 1, 1, 1 };
}