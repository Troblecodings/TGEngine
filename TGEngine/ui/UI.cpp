#include "UI.hpp"

UIComponent scene_component = UIComponent({});

UIComponent::UIComponent(glm::vec4 rect)
{
	this->rect = rect;
}

void UIComponent::draw(IndexBuffer* index, VertexBuffer* vertex)
{
	if (this->draw_call != nullptr) {
		this->draw_call(this, index, vertex);
	}
	for each (UIComponent* comp in this->ui_children)
	{
		ASSERT_NONE_NULL_DB(comp, "UIComponent null", TG_ERR_UI_COMPONENT_NULL)
		comp->draw(index, vertex);
	}
}

void UIComponent::update(int mouse_x, int mouse_y)
{
	if (this->update_call != nullptr) {
		this->update_call(this, mouse_x, mouse_y);
	}
	for each (UIComponent* comp in this->ui_children)
	{
		ASSERT_NONE_NULL_DB(comp, "UIComponent null", TG_ERR_UI_COMPONENT_NULL)
		comp->update(mouse_x, mouse_y);
	}
}

glm::vec4 UIComponent::getRect()
{
	return this->rect;
}
