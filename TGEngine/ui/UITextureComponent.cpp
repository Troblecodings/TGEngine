#include "UITextureComponent.hpp"

namespace tge {
	namespace ui {

		void UITextureComponent::draw(IndexBuffer* index, VertexBuffer* vertex) {
			if(this->offsetIndex == -1) {
				gmc::RenderOffsets offset;
				offset.material = this->materialIndex;
				offset.size = 6;
				offset.offset = index->index_count;

				TG_VECTOR_APPEND_NORMAL(gmc::render_offset, offset)
					this->offsetIndex = (uint32_t)last_size;
			}

			glm::vec2 pos = this->parent->getPosition();
			glm::vec2 ext = this->parent->getExtent();

			uint32_t idcount = (uint32_t)vertex->pointCount;

			vertex->add({ pos.x, pos.y })->add({ 0, 0 })->add(this->color);
			vertex->add({ pos.x + ext.x, pos.y })->add({ 1, 0 })->add(this->color);
			vertex->add({ pos.x + ext.x, pos.y + ext.y })->add({ 1, 1 })->add(this->color);
			vertex->add({ pos.x, pos.y + ext.y })->add({ 0, 1 })->add(this->color);

			index->addIndex(idcount);
			index->addIndex(idcount + 1);
			index->addIndex(idcount + 2);
			index->addIndex(idcount);
			index->addIndex(idcount + 2);
			index->addIndex(idcount + 3);
		}

		void UITextureComponent::init() {
			if(this->materialIndex == -1) {
				this->mat = gmc::UIMaterial(this->texture, this->color);
				TG_VECTOR_APPEND_NORMAL(gmc::materiallist, &this->mat)
					this->materialIndex = (uint32_t)last_size;
			}
		}
	}
}