#include "Mesh.hpp"

void Mesh::consume(VertexBuffer * vrt, IndexBuffer * ind) {
	this->first_index = ind->index_count;
	for (size_t i = 0; i < this->offsets.size(); i++)
	{
		this->offsets[i].offset += this->first_index;
		render_offset.push_back(this->offsets[i]);
	}
	uint32_t vertex_offset = (uint32_t)vrt->count_of_points;
	vrt->addAll(this->vertices.data(), this->vertices.size());
	for(uint32_t nt : this->indices)
	{
		ind->addIndex(vertex_offset + nt);
	}
}

void Mesh::operator<<(TGVertex vert)
{
	this->add(vert);
}

void Mesh::add(TGVertex vert)
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
