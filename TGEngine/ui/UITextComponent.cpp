#include "UITextComponent.hpp"

namespace tge {
	namespace ui {

		UITextComponent::UITextComponent(fnt::Font* font, char* string, glm::vec4 fontColor) {
			this->font = font;
			this->string = string;
			this->fontColor = fontColor;
		}

		void UITextComponent::draw(IndexBuffer* index, VertexBuffer* vertex) {
			glm::vec2 ext = this->font->getExtent(this->string);
			glm::vec2 pos = this->parent->getPosition();
			this->font->drawString(pos, this->string, vertex, index, this->fontColor);
		}

	}
}
