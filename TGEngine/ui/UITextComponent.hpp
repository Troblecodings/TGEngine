#pragma once

#include "UIDrawable.hpp"
#include "../io/Font.hpp"

namespace tge {
	namespace ui {

		/*
		 * A component for text rendering on the UI
		 * with the bounds of the font (ignores parent bounds)
		 *	-----------------------------------------------------------
		 *  Currently this adds the render offset on the first draw call
		 *  this behavior is probably going to change
		 *  -> The material is created in for each font
		 *     for that the color nor the texture can be changed
		 *     while runtime in contrast to the position which
		 *     can be changed
		 *	-----------------------------------------------------------
		 */
		class UITextComponent : public UIDrawable {

			private:
				glm::vec4 fontColor;
				char* string;
				fnt::Font* font;
				uint32_t index = -1;

			public:
				UITextComponent(fnt::Font* font, char* string) : UITextComponent(font, string, glm::vec4(1, 1, 1, 1)) {}
				UITextComponent(fnt::Font* font, char* string, glm::vec4 font_color);

			protected:
				virtual void draw(IndexBuffer* index, VertexBuffer* vertex) override;

		};
	}
}