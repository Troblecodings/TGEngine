#include "UITextureComponent.hpp"

namespace tge {
	namespace ui {

		void UITextureComponent::draw(IndexBuffer* index, VertexBuffer* vertex) {
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

	}
}