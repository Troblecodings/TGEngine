#include "UI.h"

UIComponent scene_component;

void UIComponent::draw(IndexBuffer* index, VertexBuffer* vertex)
{
	if (this->draw_call != nullptr) {
		this->draw_call(this, index, vertex);
	}
	for (size_t i = 0; i < this->ui_children.size(); i++)
	{
		UIComponent* comp = static_cast<UIComponent*>(this->ui_children[i]);
		ASSERT_NONE_NULL_DB(comp, "UIComponent null in " << __FILE__, TG_ERR_UI_COMPONENT_NULL)
		comp->draw(index, vertex);
	}
}

void UIComponent::update(int mouse_x, int mouse_y)
{
	if (this->update_call != nullptr) {
		this->update_call(this, mouse_x, mouse_y);
	}
	for (size_t i = 0; i < this->ui_children.size(); i++)
	{
		UIComponent* comp = static_cast<UIComponent*>(this->ui_children[i]);
		ASSERT_NONE_NULL_DB(comp, "UIComponent null in " << __FILE__, TG_ERR_UI_COMPONENT_NULL)
		comp->update(mouse_x, mouse_y);
	}
}
