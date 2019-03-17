#include "Mesh.hpp"

void Mesh::consume(VertexBuffer * vrt, IndexBuffer * ind) {
	this->first_index = ind->index_count;
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
	createDesctiptorLayout();
	createPipelineLayout(1, &descriptor_set_layouts[last_size]);
	this->layout_index = last_size;
	createDescriptorSet(this->layout_index);
	this->descriptor_index = light_buffer.descriptor.descriptor_set = camera_uniform.descriptor.descriptor_set = last_size;

	createPipeline(infos, 2);
	this->pipeline_index = last_size;

	camera_uniform.descriptor.binding = 0;
	fillUniformBuffer(&camera_uniform, &glm::mat4(1.0f), sizeof(glm::mat4));
	updateDescriptorSet(&camera_uniform.descriptor, sizeof(glm::mat4));

	light_buffer.descriptor.binding = 1;
	updateDescriptorSet(&light_buffer.descriptor, sizeof(glm::vec3));

	VkDescriptorImageInfo info;
	info.imageView = this->texture->image_view;
	info.sampler = tex_image_sampler;
	info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	vlib_descriptor_writes.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	vlib_descriptor_writes.dstBinding = 2;
	vlib_descriptor_writes.dstSet = descriptor_set[this->descriptor_index];
	vlib_descriptor_writes.pImageInfo = &info;
	vkUpdateDescriptorSets(device, 1, &vlib_descriptor_writes, 0, nullptr);

}
