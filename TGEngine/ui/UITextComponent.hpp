#pragma once

#include "UI.hpp"

namespace tg_ui {
	
	// TODO implement constructor with anchor

	class UITextComponent : public UIComponent
	{
	public:
		UITextComponent(Font* font, char* string);
		UITextComponent(Font* font, glm::vec4 font_color, char* string);

	protected:
		virtual void draw(IndexBuffer* index, VertexBuffer* vertex) override;

	private:
		glm::vec4 font_color;
		char* string;
		Font* font;
	};
}