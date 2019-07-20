#include "Mesh.hpp"

namespace tge {
	namespace gmc {

		void Mesh::consume(VertexBuffer* vrt, IndexBuffer* ind) {
			this->first_index = ind->index_count;
			uint32_t vertex_offset = (uint32_t)vrt->pointCount;
			vrt->addAll(this->vertices.data(), this->vertices.size() * VERTEX_SIZE, this->vertices.size());
			for(uint32_t nt : this->indices) {
				ind->addIndex(vertex_offset + nt);
			}
		}

		void Mesh::operator<<(TGVertex vert) {
			this->add(vert);
		}

		void Mesh::add(TGVertex vert) {
			for(size_t b = 0; b < this->vertices.size(); b++) {
				if(this->vertices[b] == vert) {
					this->indices.push_back((uint32_t)b);
					return;
				}
			}
			last_size = (uint32_t)this->vertices.size();
			this->vertices.push_back(vert);
			this->indices.push_back((uint32_t)last_size);
		}
	}
}
