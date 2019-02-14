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
		float div = window_list[0]->width - window_list[0]->height;
		div /= (float)window_list[0]->height;
		div += 1;
		glm::vec2 pos = (this->parent->getPosition() * glm::vec2(2, 2)) - glm::vec2(1, 1);
		pos *= glm::vec2(div, 1);
		glm::vec2 ext = this->parent->extent;
		ext *= glm::vec2(multiplier, 1);
		drawRectangle(glm::vec3(pos, 0.1f), this->color, this->texture->index, ext.x, ext.y, vertex, index);
	}
}