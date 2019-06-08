#include "UIDrawable.hpp"

namespace tge {
	namespace ui {

		void UIDrawable::draw(IndexBuffer* index, VertexBuffer* vertex)
		{
		}

		void UIDrawable::init()
		{
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