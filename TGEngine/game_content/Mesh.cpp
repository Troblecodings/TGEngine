#include "Mesh.hpp"

void Mesh::consume(VertexBuffer * vrt, IndexBuffer * ind) {
	uint32_t sz = (uint32_t)vrt->count_of_points;
	vrt->addAll(this->vertices.data(), this->vertices.size());
	for(uint32_t nt : this->indices)
	{
		ind->addIndex(sz + nt);
	}
}

void Mesh::operator<<(TGVertex vert)
{
	for (size_t b = 0; b < this->vertices.size(); b++)
	{
		if (this->vertices[b] == vert) {
			this->indices.push_back((uint32_t)b);
			return;
		}
	}
	last_size = (uint32_t)this->vertices.size();
	this->vertices.push_back(vert);
	this->indices.push_back((uint32_t)last_size);
}

