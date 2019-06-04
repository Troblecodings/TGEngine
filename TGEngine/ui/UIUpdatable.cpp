#include "UIUpdatable.hpp"

namespace tge {
	namespace ui {

		void UIUpdatable::onAddTo(UIEntity* parent)
		{
			if (this->parent)
				this->parent->removeUpdatable(this);
			this->parent = parent;
		}

		void UIUpdatable::onRemoveFrom(UIEntity* parent)
		{
			this->parent = nullptr;
		}
		bool UIUpdatable::isEnabled()
		{
			return this->enabled;
		}

		void UIUpdatable::setEnabled(bool enabled)
		{
			this->enabled = enabled;
		}
	}
}
