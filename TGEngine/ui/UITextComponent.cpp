#include "UITextComponent.hpp"

namespace tge {
	namespace ui {

		UITextComponent::UITextComponent(tg_font::Font* font, char* string, glm::vec4 font_color, Anchor anchor) : UIDrawable(anchor)
		{
			this->font = font;
			this->string = string;
			this->font_color = font_color;
		}

		void UITextComponent::draw(IndexBuffer* index, VertexBuffer* vertex)
		{
			if (this->index == -1) {
				RenderOffsets off;
				off.material = this->font->getMaterialIndex();
				off.offset = index->index_count;
				off.size = strlen(this->string) * 6;
				TG_VECTOR_APPEND_NORMAL(render_offset, off)
					this->index = last_size;
			}

			glm::vec2 ext = this->font->getExtent(this->string);
			glm::vec2 pos = this->parent->getPosition();
			switch (anchor)
			{
			case TOP_LEFT:
				pos.y += ext.y;
				break;
			case TOP_CENTER:
				pos.y += ext.y;
				pos.x -= ext.x / 2;
				break;
			case TOP_RIGHT:
				pos.y += ext.y;
				pos.x -= ext.x;
				break;
			case CENTER_LEFT:
				pos.y += ext.y / 2;
				break;
			case CENTER:
				pos.y += ext.y / 2;
				pos.x -= ext.x / 2;
				break;
			case CENTER_RIGHT:
				pos.y += ext.y / 2;
				pos.x -= ext.x;
				break;
			case BOTTOM_LEFT:
				break;
			case BOTTOM_CENTER:
				pos.x -= ext.x / 2;
				break;
			case BOTTOM_RIGHT:
				pos.x -= ext.x;
				break;
			default:
				break;
			}
			this->font->drawString(pos, this->string, vertex, index);
		}

	}
}
