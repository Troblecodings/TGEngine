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
		if (this->index == -1) {
			RenderOffsets off;
			off.material = this->font->getMaterialIndex();
			off.offset = index->index_count;
			off.size = strlen(this->string) * 6;
			TG_VECTOR_APPEND_NORMAL(render_offset, off)
			this->index = last_size;
		}

		glm::vec2 ext = this->font->getExtent(this->string);
		glm::vec2 pos = this->parent->getRenderPosition();
		switch (anchor)
		{
		case tg_ui::TOP_LEFT:
			pos.y += ext.y;
			break;
		case tg_ui::TOP_CENTER:
			pos.y += ext.y;
			pos.x -= ext.x / 2;
			break;
		case tg_ui::TOP_RIGHT:
			pos.y += ext.y;
			pos.x -= ext.x;
			break;
		case tg_ui::CENTER_LEFT:
			pos.y += ext.y / 2;
			break;
		case tg_ui::CENTER:
			pos.y += ext.y / 2;
			pos.x -= ext.x / 2;
			break;
		case tg_ui::CENTER_RIGHT:
			pos.y += ext.y / 2;
			pos.x -= ext.x;
			break;
		case tg_ui::BOTTOM_LEFT:
			break;
		case tg_ui::BOTTOM_CENTER:
			pos.x -= ext.x / 2;
			break;
		case tg_ui::BOTTOM_RIGHT:
			pos.x -= ext.x;
			break;
		default:
			break;
		}
		this->font->drawString(pos, this->string, vertex, index);
	}

}
