#pragma once

#include "UI.hpp"

namespace tg_ui {
	
	class UITextComponent : public UIComponent
	{
	public:
		UITextComponent(tg_font::Font* font, char* string) : UITextComponent(font, string, glm::vec4(1, 1, 1, 1)) {}
		UITextComponent(tg_font::Font* font, char* string, glm::vec4 font_color) : UITextComponent(font, string, font_color, Anchor::TOP_LEFT) {}
		UITextComponent(tg_font::Font* font, char* string, glm::vec4 font_color, Anchor anchor);

	protected:
		virtual void draw(IndexBuffer* index, VertexBuffer* vertex) override;

	private:
		glm::vec4 font_color;
		char* string;
		tg_font::Font* font;
	};
}