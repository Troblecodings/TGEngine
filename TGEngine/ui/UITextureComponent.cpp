#include "UITextureComponent.hpp"

namespace tg_ui {

	UITextureComponent::UITextureComponent(Texture * tex) : UITextureComponent(tex, glm::vec4(1, 1, 1, 1)) {}

	UITextureComponent::UITextureComponent(Texture * tex, glm::vec4 color)
	{
		this->texture = tex;
		this->color = color;
	}

	void UITextureComponent::draw(IndexBuffer * index, VertexBuffer * vertex)
	{
		glm::vec2 pos = this->parent->getRenderPosition();
		glm::vec2 ext = this->parent->getRenderExtent();
		drawRectangle(glm::vec3(pos, 0.1f), ext.x, ext.y, vertex, index);
	}
	void UITextureComponent::update(int mouse_x, int mouse_y)
	{
		this->parent->local_position += glm::vec2(0.00002, 0.00002);
	}

	void UITextureComponent::init()
	{
	}
}