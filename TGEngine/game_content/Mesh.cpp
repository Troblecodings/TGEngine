#include "Mesh.hpp"

void Mesh::consume(VertexBuffer * vrt, IndexBuffer * ind) {
	uint32_t sz = (uint32_t)vrt->count_of_points;
	vrt->addAll(this->vertices.data(), this->vertices.size());
	for(uint32_t nt : this->indices)
	{
		ind->addIndex(sz + nt);
	}
}
