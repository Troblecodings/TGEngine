#include "UITextureComponent.hpp"

namespace tge {
	namespace ui {

		void UITextureComponent::draw(IndexBuffer* index, VertexBuffer* vertex)
		{
			if (this->offsetIndex == -1) {
				RenderOffsets offset;
				offset.material = this->materialIndex;
				offset.size = 6;
				offset.offset = index->index_count;

				TG_VECTOR_APPEND_NORMAL(render_offset, offset)
					this->offsetIndex = (uint32_t)last_size;
			}

			glm::vec2 pos = this->parent->getPosition();
			glm::vec2 ext = this->parent->getExtent();

			uint32_t idcount = (uint32_t)vertex->count_of_points;

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
		}

		void UITextureComponent::init()
		{
			if (this->materialIndex == -1) {
				Material mat = Material(this->texture, this->color);
				TG_VECTOR_APPEND_NORMAL(materials, &mat)
				this->materialIndex = (uint32_t)last_size;
			}
		}
	}
}