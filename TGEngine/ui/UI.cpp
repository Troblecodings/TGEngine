#include "UI.hpp"

UIEntity ui_scene_entity = UIEntity({});

UIEntity::UIEntity(TGRect rect)
{
	this->local_bounds = rect;
}

void UIEntity::addComponent(UIComponent* component)
{
	this->components.push_back(component);
	component->onAddTo(this);
}

void UIEntity::addChildren(UIEntity* children)
{
	this->children.push_back(children);
	children->onAddTo(this);
}

void UIEntity::removeComponent(UIComponent* component)
{
	this->components.erase(std::remove(this->components.begin(), this->components.end(), component));
	component->onRemoveFrom(this);
}

void UIEntity::removeChildren(UIEntity* children)
{
	this->children.erase(std::remove(this->children.begin(), this->children.end(), children));
	children->onRemoveFrom(children);
}

void UIEntity::draw(IndexBuffer* index, VertexBuffer* vertex)
{
	for each (UIComponent* comp in this->components)
	{
		ASSERT_NONE_NULL_DB(comp, "UIComponent null", TG_ERR_UI_COMPONENT_NULL)
		comp->draw(index, vertex);
	}
	for each (UIEntity* entity in this->children)
	{
		ASSERT_NONE_NULL_DB(entity, "UIEntity null", TG_ERR_UI_ENTITY_NULL)
		entity->draw(index, vertex);
	}
}

void UIEntity::update(int mouse_x, int mouse_y)
{
	for each (UIComponent* comp in this->components)
	{
		ASSERT_NONE_NULL_DB(comp, "UIComponent null", TG_ERR_UI_COMPONENT_NULL)
		comp->update(mouse_x, mouse_y);
	}
	for each (UIEntity* entity in this->children)
	{
		ASSERT_NONE_NULL_DB(entity, "UIEntity null", TG_ERR_UI_ENTITY_NULL)
		entity->update(mouse_x, mouse_y);
	}
}

void UIEntity::onAddTo(UIEntity* parent)
{
	this->parent = parent;
}

void UIEntity::onRemoveFrom(UIEntity* parent)
{
	this->parent = nullptr;
}

TGRect UIEntity::getBounds()
{
	TGRect rect = this->local_bounds;
	TGRect parent = this->parent->getBounds();
	rect.x += parent.x;
	rect.y += parent.y;
	return rect;
}

void UIEntity::UIComponent::onAddTo(UIEntity* parent)
{
	if (this->parent) {
		this->parent->removeComponent(this);
	}
	this->parent = parent;
}

void UIEntity::UIComponent::onRemoveFrom(UIEntity* parent)
{
	this->parent = nullptr;
}
