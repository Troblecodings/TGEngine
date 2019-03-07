#pragma once

#include "UI.hpp"

namespace tg_ui {

	class UITextureComponent : public UIComponent {

	public:
		UITextureComponent(Texture* texture);
		UITextureComponent(Texture* texture, glm::vec4 color);

	protected:
		virtual void draw(IndexBuffer* index, VertexBuffer* vertex) override;
		virtual void update(int mouse_x, int mouse_y) override;

	private:
		Texture* texture;
		glm::vec4 color;
	};

}