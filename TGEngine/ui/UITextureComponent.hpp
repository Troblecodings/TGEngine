#pragma once

#include "UIDrawable.hpp"
#include "../gamecontent/UIMaterial.hpp"

using namespace tge::tex;

namespace tge {
	namespace ui {

		/*
		 * A component for texture rendering on the UI
		 * with the bounds of the entity
		 *	-----------------------------------------------------------
		 *  Currently this creates a new material in the init call
		 *  and adds the render offset on the first draw call
		 *  this behavior is probably going to change
		 *  -> for that the color nor the texture can be changed
		 *     while runtime in contrast to the position which
		 *     can be changed
		 *	-----------------------------------------------------------
		 */
		SINCE(0, 0, 4)
		class UITextureComponent : public UIDrawable {

		private:
			Texture* texture = nullptr;
			UIMaterial mat = UIMaterial((Texture*)nullptr);
			glm::vec4 color = glm::vec4(1.0f);
			uint32_t materialIndex = -1;
			uint32_t offsetIndex = -1;

		public:
			UITextureComponent(Texture* texture) : UITextureComponent(texture, glm::vec4(1.0f)) {}
			UITextureComponent(Texture* texture, glm::vec4 color) : texture(texture), color(color) {}

		protected:
			virtual void draw(IndexBuffer* index, VertexBuffer* vertex) override;
			virtual void init() override;
		};

	}
}