#include "UI.h"

UIComponent scene_component;

void UIComponent::draw(IndexBuffer* index, VertexBuffer* vertex)
{
	if (this->draw_call != nullptr) {
		this->draw_call(this, index, vertex);
	}
	for each (UIComponent var in this->ui_children)
	{
		var.draw(index, vertex);
	}
}

void UIComponent::update(int mouse_x, int mouse_y)
{
	if (this->update_call != nullptr) {
		this->update_call(this, mouse_x, mouse_y);
	}
	for each (UIComponent var in this->ui_children)
	{
		var.update(mouse_x, mouse_y);
	}
}
