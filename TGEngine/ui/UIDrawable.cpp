#include "UIDrawable.hpp"

namespace tge {
	namespace ui {

		void UIDrawable::draw(IndexBuffer* index, VertexBuffer* vertex)
		{
		}

		void UIDrawable::init()
		{
		}

		void UIDrawable::update()
		{
			ASSERT_NONE_NULL_DB(this->parent, "Parent null in update call", TG_ERR_DB_NULLPTR)
			this->cachedPosition = this->parent->getPosition();
			glm::vec2 extent = this->parent->getExtent();
		}

		void UIDrawable::onAddTo(UIEntity* parent)
		{
			if (this->parent)
				this->parent->removeDrawable(this);
			this->parent = parent;
		}

		void UIDrawable::onRemoveFrom(UIEntity* parent)
		{
			this->parent = nullptr;
		}
		bool UIDrawable::isEnabled()
		{
			return this->enabled;
		}

		void UIDrawable::setEnabled(bool enabled)
		{
			this->enabled = enabled;
		}
	}
}