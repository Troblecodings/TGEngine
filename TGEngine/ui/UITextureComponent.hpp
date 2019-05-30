#pragma once

#include "UI.hpp"

namespace tg_ui {
	
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
	class UITextureComponent : public UIComponent {

	public:
		UITextureComponent(Texture* texture) : UITextureComponent(texture, glm::vec4(1.0f)) {}
		UITextureComponent(Texture* texture, glm::vec4 color) : UITextureComponent(texture, color, TOP_LEFT) {}
		UITextureComponent(Texture* texture, glm::vec4 color, Anchor anchor) : UIComponent(anchor), texture(texture), color(color) {}

	protected:
		virtual void draw(IndexBuffer* index, VertexBuffer* vertex) override;
		virtual void init() override;

	private:
		Texture* texture = nullptr;
		glm::vec4 color = glm::vec4(1.0f);
		uint32_t material_index = -1;
		uint32_t offset_index = -1;
	};

}