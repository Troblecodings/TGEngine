#pragma once

#include "UIEntity.hpp"

namespace tge {
	namespace ui {
		/*
		 * 	Component class that holds the actual draw and update calls
		 */
		SINCE(0, 0, 4)
			class UIDrawable {

			protected:
				// Parent this belongs to
				// if null this is unbound
				UIEntity* parent = nullptr;

			private:
				bool enabled = true; // if enabled this will be draw it will still recieve updates

			public:
				virtual void draw(IndexBuffer* index, VertexBuffer* vertex); // default implementation, needs override
				virtual void init(); // default implementation, needs override

				void onAddTo(UIEntity* parent);
				void onRemoveFrom(UIEntity* parent);

				// See enabled
				bool isEnabled();
				void setEnabled(bool enabled);

		};


	}
}