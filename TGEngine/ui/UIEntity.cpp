#include "UIEntity.hpp"
#include "UIDrawable.hpp"
#include "UIUpdatable.hpp"

namespace tge {
	namespace ui {
		UIEntity ui_scene_entity = UIEntity({ 0, 0 }, {});

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

		void UIEntity::setPosition(glm::vec2 position)
		{
			this->localPosition = position;
			__update();
		}

		void UIEntity::setExtent(glm::vec2 extent)
		{
			this->extent = extent;
			__update();
		}

		glm::vec2 UIEntity::__getPosition()
		{
			glm::vec2 tmp_pos = (this->localPosition * glm::vec2(2, 2)) - glm::vec2(1.0f);
			if (this->parent == nullptr) return tmp_pos;
			return this->parent->cachedPosition + tmp_pos;
		}

		void UIEntity::__update() {
			this->cachedPosition = this->__getPosition();
			this->cachedExtent = this->extent * glm::vec2(2 * multiplier, 2);

			// TODO Anchor

			for each (UIEntity* entity in this->children)
			{
				entity->__update();
			}

			for each (UIDrawable * drawable in this->drawComponents) {
				drawable->update();
			}
		}
	}
}
