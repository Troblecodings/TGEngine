#include "UITextComponent.hpp"

namespace tg_ui {

	UITextComponent::UITextComponent(tg_font::Font* font, char* string, glm::vec4 font_color, Anchor anchor) : UIComponent(anchor)
	{
		this->font = font;
		this->string = string;
		this->font_color = font_color;
	}

	void UITextComponent::draw(IndexBuffer * index, VertexBuffer * vertex)
	{
		this->font->drawString(glm::vec3(this->parent->getPosition(), 0.1f), this->font_color, this->string, vertex, index);
	}

}
