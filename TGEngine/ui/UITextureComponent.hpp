#pragma once

#include "UI.hpp"

namespace tg_ui {

	class UITextureComponent : public UIComponent {

	public:
		UITextureComponent(Texture* texture) : UITextureComponent(texture, glm::vec4(1.0f)) {}
		UITextureComponent(Texture* texture, glm::vec4 color) : UITextureComponent(texture, color, TOP_LEFT) {}
		UITextureComponent(Texture* texture, glm::vec4 color, Anchor anchor) : UIComponent(anchor), texture(texture), color(color) {}

	protected:
		virtual void draw(IndexBuffer* index, VertexBuffer* vertex) override;
		virtual void update(int mouse_x, int mouse_y) override;
		virtual void init() override;

	private:
		Texture* texture;
		glm::vec4 color;
		uint32_t material_index = -1;
		uint32_t offset_index = -1;
	};

}