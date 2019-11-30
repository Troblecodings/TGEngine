#pragma once

#include "UIEntity.hpp"

namespace tge {
	namespace ui {
		/*
		 * 	Component class that holds the actual draw and update calls
		 */
		class UIUpdatable {

			protected:
				UIEntity* parent = nullptr;

			private:
				bool enabled = true;

			public:
				UIUpdatable() {}

				virtual void update(); // default implementation, needs override

				void onAddTo(UIEntity* parent);
				void onRemoveFrom(UIEntity* parent);

				bool isEnabled();
				void setEnabled(bool enabled);
		};


	}
}