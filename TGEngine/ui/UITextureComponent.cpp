#include "UITextureComponent.hpp"

namespace tg_ui {

	void UITextureComponent::draw(IndexBuffer * index, VertexBuffer * vertex)
	{
		if (this->offset_index == -1) {
			Material mat;
			mat.color = this->color;
			mat.texture = this->texture;
			vlib_depth_stencil_state.depthTestEnable = VK_FALSE;
			vlib_rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
			mat.createMaterial();
			vlib_rasterization_state.cullMode = VK_CULL_MODE_FRONT_BIT;
			vlib_depth_stencil_state.depthTestEnable = VK_TRUE;
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

		uint32_t idcount = (uint32_t)vertex->count_of_points;

		switch (this->anchor)
		{
		case TOP_LEFT:
			break;
		case TOP_CENTER:
			pos.x -= ext.x / 2;
			break;
		case TOP_RIGHT:
			pos.x -= ext.x;
			break;
		case CENTER_LEFT:
			pos.y -= (ext.y / 2);
			break;
		case CENTER:
			pos -= (ext / 2.0f);
			break;
		case CENTER_RIGHT:
			pos -= (ext / glm::vec2(2, 1));
			break;
		case BOTTOM_LEFT:
			pos.y -= ext.y;
			break;
		case BOTTOM_CENTER:
			pos -= (ext / glm::vec2(1, 2));
			break;
		case BOTTOM_RIGHT:
			pos -= ext;
			break;
		}

		TGVertex first = { { pos.x, pos.y, 0 }, { 0, 0 } };
		TGVertex second = { { pos.x + ext.x, pos.y, 0 }, { 1, 0 } };
		TGVertex third = { { pos.x + ext.x, pos.y + ext.y, 0 }, { 1, 1 } };
		TGVertex fourth = { { pos.x, pos.y + ext.y, 0 }, { 0, 1 } };

		vertex->add(first);
		vertex->add(second);
		vertex->add(third);
		vertex->add(fourth);

		index->addIndex(idcount);
		index->addIndex(idcount + 1);
		index->addIndex(idcount + 2);
		index->addIndex(idcount);
		index->addIndex(idcount + 2);
		index->addIndex(idcount + 3);


		// drawRectangle(glm::vec3(pos, 0.1f), ext.x, ext.y, vertex, index);
	}
	void UITextureComponent::update(int mouse_x, int mouse_y)
	{
	}

	void UITextureComponent::init()
	{
	}
}