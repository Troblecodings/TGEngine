#include "UITextureComponent.hpp"

namespace tg_ui {

	UITextureComponent::UITextureComponent(Texture * tex)
	{
		this->texture = tex;
		this->color = glm::vec4(1, 1, 1, 1);
	}

	UITextureComponent::UITextureComponent(Texture * tex, glm::vec4 color)
	{
		this->texture = tex;
		this->color = color;
	}

	void UITextureComponent::draw(IndexBuffer * index, VertexBuffer * vertex)
	{
		glm::vec2 pos = this->parent->getRenderPosition();
		glm::vec2 ext = this->parent->extent;
		drawRectangle(glm::vec3(pos, 0.1f), this->color, this->texture->index, ext.x, ext.y, vertex, index);
	}
	void UITextureComponent::update(int mouse_x, int mouse_y)
	{
	}
}