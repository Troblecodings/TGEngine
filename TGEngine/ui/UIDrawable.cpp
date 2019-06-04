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

			switch (this->anchor)
			{
			case TOP_LEFT:
				break;
			case TOP_CENTER:
				this->cachedPosition.x -= extent.x / 2;
				break;
			case TOP_RIGHT:
				this->cachedPosition.x -= extent.x;
				break;
			case CENTER_LEFT:
				this->cachedPosition.y -= (extent.y / 2);
				break;
			case CENTER:
				this->cachedPosition -= (extent / 2.0f);
				break;
			case CENTER_RIGHT:
				this->cachedPosition -= (extent / glm::vec2(1, 2));
				break;
			case BOTTOM_LEFT:
				this->cachedPosition.y -= extent.y;
				break;
			case BOTTOM_CENTER:
				this->cachedPosition -= (extent / glm::vec2(2, 1));
				break;
			case BOTTOM_RIGHT:
				this->cachedPosition -= extent;
				break;
			}
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