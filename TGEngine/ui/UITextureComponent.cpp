#include "UITextureComponent.hpp"

namespace tg_ui {

	UITextureComponent::UITextureComponent(Texture * tex) : UITextureComponent(tex, glm::vec4(1, 1, 1, 1)) {}

	UITextureComponent::UITextureComponent(Texture * tex, glm::vec4 color)
	{
		this->texture = tex;
		this->color = color;
	}

	void UITextureComponent::draw(IndexBuffer * index, VertexBuffer * vertex)
	{
		if (this->offset_index == -1) {
			Material mat;
			mat.color = this->color;
			mat.texture = this->texture;
			vlib_rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
			mat.createMaterial();
			vlib_rasterization_state.cullMode = VK_CULL_MODE_FRONT_BIT;
			ui_camera_uniform.descriptor.descriptorset = mat.descriptor_index;
			ui_camera_uniform.descriptor.binding = 0;
			ui_camera_uniform.updateDescriptor();

			TG_VECTOR_APPEND_NORMAL(materials, mat)

			RenderOffsets offset;
			offset.material = last_size;
			offset.size = 6;
			offset.offset = index->index_count;

			TG_VECTOR_APPEND_NORMAL(render_offset, offset)
			this->offset_index = last_size;
		}
		glm::vec2 pos = this->parent->getRenderPosition();
		glm::vec2 ext = this->parent->getRenderExtent();
		drawRectangle(glm::vec3(pos, 0.1f), ext.x, ext.y, vertex, index);
	}
	void UITextureComponent::update(int mouse_x, int mouse_y)
	{
	}

	void UITextureComponent::init()
	{
	}
}