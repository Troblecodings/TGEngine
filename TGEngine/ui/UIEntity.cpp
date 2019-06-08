#include "UIEntity.hpp"
#include "UIDrawable.hpp"
#include "UIUpdatable.hpp"

namespace tge {
	namespace ui {
		UIEntity ui_scene_entity = UIEntity({ 0, 0 }, { 1, 1 });

		void UIEntity::addDrawable(UIDrawable* component)
		{
			ASSERT_NONE_NULL_DB(component, "UIComponent null", TG_ERR_DB_NULLPTR)
			this->drawComponents.push_back(component);
			component->onAddTo(this);
		}

		void UIEntity::addUpdatable(UIUpdatable* component)
		{
			ASSERT_NONE_NULL_DB(component, "UIComponent null", TG_ERR_DB_NULLPTR)
			this->updateComponents.push_back(component);
			component->onAddTo(this);
		}

		void UIEntity::addChildren(UIEntity* children)
		{
			ASSERT_NONE_NULL_DB(children, "UIEntity null", TG_ERR_DB_NULLPTR)
			this->children.push_back(children);
			children->onAddTo(this);
		}

		void UIEntity::removeDrawable(UIDrawable* component)
		{
			this->drawComponents.erase(std::remove(this->drawComponents.begin(), this->drawComponents.end(), component));
			component->onRemoveFrom(this);
		}

		void UIEntity::removeUpdatable(UIUpdatable* component)
		{
			this->updateComponents.erase(std::remove(this->updateComponents.begin(), this->updateComponents.end(), component));
			component->onRemoveFrom(this);
		}

		void UIEntity::removeChildren(UIEntity* children)
		{
			this->children.erase(std::remove(this->children.begin(), this->children.end(), children));
			children->onRemoveFrom(children);
		}

		void UIEntity::draw(IndexBuffer* index, VertexBuffer* vertex)
		{
			for each (UIDrawable* comp in this->drawComponents)
			{
				ASSERT_NONE_NULL_DB(comp, "UIComponent null", TG_ERR_DB_NULLPTR)
					if (comp->isEnabled())
						comp->draw(index, vertex);
			}
			for each (UIEntity * entity in this->children)
			{
				ASSERT_NONE_NULL_DB(entity, "UIEntity null", TG_ERR_DB_NULLPTR)
					if (entity->isEnabled() && entity->isVisible())
						entity->draw(index, vertex);
			}
		}

		void UIEntity::update()
		{
			for each (UIUpdatable * comp in this->updateComponents)
			{
				ASSERT_NONE_NULL_DB(comp, "UIComponent null", TG_ERR_DB_NULLPTR)
					if (comp->isEnabled())
						comp->update();
			}
			for each (UIEntity * entity in this->children)
			{
				ASSERT_NONE_NULL_DB(entity, "UIEntity null", TG_ERR_DB_NULLPTR)
					if (entity->isEnabled())
						entity->update();
			}
		}

		void UIEntity::init()
		{
			if (parent == nullptr)
	            this->__update();
			for each (UIDrawable * comp in this->drawComponents)
			{
				ASSERT_NONE_NULL_DB(comp, "UIComponent null", TG_ERR_DB_NULLPTR)
					comp->init();
			}
			for each (UIEntity * entity in this->children)
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
			case TOP_LEFT:
				break;
			case TOP_CENTER:
				this->localPosition.x = this->parent->extent.x / 2;
				break;
			case TOP_RIGHT:
				this->localPosition.x = this->parent->extent.x;
				break;
			case CENTER_LEFT:
				this->localPosition.y = (this->parent->extent.y / 2);
				break;
			case CENTER:
				this->localPosition.x = this->parent->extent.x / 2;
				this->localPosition.y = (this->parent->extent.y / 2);
				break;
			case CENTER_RIGHT:
				this->localPosition = (this->parent->extent / glm::vec2(1, 2));
				break;
			case BOTTOM_LEFT:
				this->localPosition.y = this->parent->extent.y;
				break;
			case BOTTOM_CENTER:
				this->localPosition.x = this->parent->extent.x / 2;
				this->localPosition.y = this->parent->extent.y;
				break;
			case BOTTOM_RIGHT:
				this->localPosition = this->parent->extent;
				break;
			}

			this->__update();
		}

		void UIEntity::onRemoveFrom(UIEntity* parent)
		{
			this->parent = nullptr;
			this->__update();
		}

		bool UIEntity::isEnabled()
		{
			return this->enabled;
		}

		void UIEntity::setEnabled(bool enabled)
		{
			this->enabled = enabled;
		}

		bool UIEntity::isVisible()
		{
			return this->visible;
		}

		void UIEntity::setVisible(bool visible)
		{
			this->visible = visible;
		}

		glm::vec2 UIEntity::getPosition()
		{
			return this->cachedPosition;
		}

		glm::vec2 UIEntity::getExtent()
		{
			return this->cachedExtent;
		}

		glm::vec2 UIEntity::getLocalPosition()
		{
			return this->cachedPosition;
		}

		glm::vec2 UIEntity::getLocalExtent()
		{
			return this->cachedExtent;
		}

		glm::vec2 UIEntity::getOffset()
		{
			return this->offsetPoint;
		}

		void UIEntity::setPosition(glm::vec2 position)
		{
			this->localPosition = position;
			this->__update();
		}

		void UIEntity::setExtent(glm::vec2 extent)
		{
			this->extent = extent;
			this->__update();
		}

		void UIEntity::setOffset(glm::vec2 offset)
		{
			this->offsetPoint = offset;
			this->__update();
		}

		void UIEntity::setOffset(Anchor anchor)
		{
			switch (this->anchor){
            case TOP_LEFT:
            	break;
            case TOP_CENTER:
            	this->setOffset(glm::vec2(this->extent.x / 2, 0));
            	break;
            case TOP_RIGHT:
				this->setOffset(glm::vec2(this->extent.x, 0));
            	break;
            case CENTER_LEFT:
				this->setOffset(glm::vec2(0, this->extent.y / 2));
            	break;
            case CENTER:
				this->setOffset(glm::vec2(this->extent / 2.0f));
            	break;
            case CENTER_RIGHT:
				this->setOffset(glm::vec2(this->extent.x, this->extent.y / 2));
            	break;
            case BOTTOM_LEFT:
				this->setOffset(glm::vec2(0, this->extent.y));
            	break;
            case BOTTOM_CENTER:
				this->setOffset(glm::vec2(this->extent.x / 2, this->extent.y));
				break;
            case BOTTOM_RIGHT:
				this->setOffset(glm::vec2(this->extent.x, this->extent.y));
				break;
            }
		}

		glm::vec2 UIEntity::__getPosition()
		{
			if (this->parent == nullptr) 
				return this->localPosition - this->__getOffset();
			return this->parent->__getPosition() + this->localPosition - this->__getOffset();
		}

		glm::vec2 UIEntity::__getOffset() {
			return glm::vec2(this->offsetPoint.x * multiplier, this->offsetPoint.y);
		}

		void UIEntity::__update() {
			this->cachedPosition = this->__getPosition() * glm::vec2(2, 2) - glm::vec2(1.0f);
			this->cachedExtent = this->extent * glm::vec2(2 * multiplier, 2);

			for each (UIEntity* entity in this->children)
			{
				entity->__update();
			}
		}
	}
}
