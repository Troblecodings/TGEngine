#include "Mesh.hpp"

void Mesh::consume(VertexBuffer * vrt, IndexBuffer * ind) {
	this->index_offset = ind->index_count;
	this->vertex_offset = vrt->count_of_points;
	vrt->addAll(this->vertices.data(), this->vertices.size());
	for(uint32_t nt : this->indices)
	{
		ind->addIndex(this->vertex_offset + nt);
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

bool Material::operator==(const Material & material)
{
	return material.color == this->color && material.texture == this->texture;
}

void Material::createMaterialPipeline()
{
	VkPipelineShaderStageCreateInfo* infos;

	if (this->texture) {
		infos = new VkPipelineShaderStageCreateInfo[2];
		infos[0] = shaders[TG_VERTEX_SHADER_TEXTURED_INDEX];
		infos[1] = shaders[TG_FRAGMENT_SHADER_TEXTURED_INDEX];
	}
	else {
		infos = new VkPipelineShaderStageCreateInfo[2];
		infos[0] = shaders[TG_VERTEX_SHADER_COLOR_ONLY_INDEX];
		infos[1] = shaders[TG_FRAGMENT_SHADER_COLOR_ONLY_INDEX];
	}
	createPipeline(infos, 2);
	this->pipeline_index = last_size;
}
