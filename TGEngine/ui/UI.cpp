#include "UI.hpp"

namespace tg_ui {

	UIEntity ui_scene_entity = UIEntity({0, 0}, {});

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
			ASSERT_NONE_NULL_DB(comp, "UIComponent null", TG_ERR_DB_NULLPTR)
			if (comp->isEnabled())
				comp->draw(index, vertex);
		}
		for each (UIEntity* entity in this->children)
		{
			ASSERT_NONE_NULL_DB(entity, "UIEntity null", TG_ERR_DB_NULLPTR)
			if (entity->isEnabled())
				entity->draw(index, vertex);
		}
	}

	void UIEntity::update()
	{
		for each (UIComponent* comp in this->components)
		{
			ASSERT_NONE_NULL_DB(comp, "UIComponent null", TG_ERR_DB_NULLPTR)
			if (comp->isEnabled())
				comp->update();
		}
		for each (UIEntity* entity in this->children)
		{
			ASSERT_NONE_NULL_DB(entity, "UIEntity null", TG_ERR_DB_NULLPTR)
			if(entity->isEnabled())
				entity->update();
		}
	}

	void UIEntity::init()
	{
		for each (UIComponent* comp in this->components)
		{
			ASSERT_NONE_NULL_DB(comp, "UIComponent null", TG_ERR_DB_NULLPTR)
				comp->init();
		}
		for each (UIEntity* entity in this->children)
		{
			ASSERT_NONE_NULL_DB(entity, "UIEntity null", TG_ERR_DB_NULLPTR)
				entity->init();
		}
	}

	void UIEntity::onAddTo(UIEntity* parent)
	{
		this->parent = parent;
		switch (this->anchor)
		{
		case tg_ui::TOP_LEFT:
			break;
		case tg_ui::TOP_CENTER:
			this->local_position.x = parent->extent.x / 2;
			break;
		case tg_ui::TOP_RIGHT:
			this->local_position.x = parent->extent.x;
			break;
		case tg_ui::CENTER_LEFT:
			this->local_position.y = parent->extent.y / 2;
			break;
		case tg_ui::CENTER:
			this->local_position = parent->extent / 2.0f;
			break;
		case tg_ui::CENTER_RIGHT:
			this->local_position.y = parent->extent.y / 2;
			this->local_position.x = parent->extent.x;
			break;
		case tg_ui::BOTTOM_LEFT:
			this->local_position.y = parent->extent.y;
			break;
		case tg_ui::BOTTOM_CENTER:
			this->local_position.y = parent->extent.y;
			this->local_position.x = parent->extent.x / 2;
			break;
		case tg_ui::BOTTOM_RIGHT:
			this->local_position.y = parent->extent.y;
			this->local_position.x = parent->extent.x;
			break;
		default:
			break;
		}

	}

	void UIEntity::onRemoveFrom(UIEntity* parent)
	{
		this->parent = nullptr;
	}

	bool UIEntity::isEnabled()
	{
		return this->enabled;
	}

	void UIEntity::setEnabled(bool enabled)
	{
		this->enabled = enabled;
	}

	glm::vec2 UIEntity::getPosition()
	{
		if (this->parent == nullptr) return this->local_position;
		return this->parent->getPosition() + this->local_position;
	}

	glm::vec2 UIEntity::getRenderPosition()
	{
		return (this->getPosition() * glm::vec2(2, 2)) - glm::vec2(1.0f);
	}

	glm::vec2 UIEntity::getRenderExtent()
	{
		return this->extent * glm::vec2(2 * multiplier, 2);
	}

	void UIComponent::draw(IndexBuffer * index, VertexBuffer * vertex)
	{
	}

	void UIComponent::update()
	{

	}

	void UIComponent::init()
	{
	}

	void UIComponent::onAddTo(UIEntity* parent)
	{
		if (this->parent) 
			this->parent->removeComponent(this);
		this->parent = parent;
	}

	void UIComponent::onRemoveFrom(UIEntity* parent)
	{
		this->parent = nullptr;
	}
	bool UIComponent::isEnabled()
	{
		return this->enabled;
	}

	void UIComponent::setEnabled(bool enabled)
	{
		this->enabled = enabled;
	}
}
