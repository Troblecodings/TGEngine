#include "Material.hpp"

void Material::createMaterial()
{
	// TODO Fix this mess
	VkPipelineShaderStageCreateInfo* shaders_for_tex = new VkPipelineShaderStageCreateInfo[2];
	shaders_for_tex[0] = shaders[TG_VERTEX_SHADER_TEXTURED_INDEX];
	shaders_for_tex[1] = shaders[TG_FRAGMENT_SHADER_TEXTURED_INDEX];

	VkSpecializationMapEntry* map_entrys = new VkSpecializationMapEntry[4];
	for (size_t i = 0; i < 4; i++)
	{
		map_entrys[i].constantID = i;
		map_entrys[i].offset = i * sizeof(float);
		map_entrys[i].size = sizeof(float);
	}

	VkSpecializationInfo pSpecialization;
	pSpecialization.mapEntryCount = 4;
	pSpecialization.pData = &this->color;
	pSpecialization.dataSize = sizeof(float) * 4;
	pSpecialization.pMapEntries = map_entrys;
	//

	shaders_for_tex[1].pSpecializationInfo = &pSpecialization;

	createPipelineLayout(1, &descriptor_set_layouts[createDesctiptorLayout()]);
	this->layout_index = last_size;

	this->descriptor_index = textureDescriptor.descriptorset = light_buffer.descriptor.descriptorset = camera_uniform.descriptor.descriptorset = createDescriptorSet(this->layout_index);

	createPipeline(shaders_for_tex, 2, this->layout_index);
	this->pipeline_index = last_size;

	camera_uniform.descriptor.binding = 0;
	camera_uniform.updateDescriptor();

	light_buffer.descriptor.binding = 1;
	light_buffer.updateDescriptor();

	this->texture->vulkanTexture.updateDescriptor();
}

void Material::createUIMaterial()
{
	vlib_depth_stencil_state.depthTestEnable = VK_FALSE;
	vlib_rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
	Material::createMaterial();
	vlib_rasterization_state.cullMode = VK_CULL_MODE_FRONT_BIT;
	vlib_depth_stencil_state.depthTestEnable = VK_TRUE;
	ui_camera_uniform.descriptor.descriptorset = this->descriptor_index;
	ui_camera_uniform.descriptor.binding = 0;
	ui_camera_uniform.updateDescriptor();
}

void Material::destroy()
{
	destroyDescriptorSet(this->descriptor_index);
	destroyPipeline(this->pipeline_index);
	destroyPipelineLayout(this->layout_index);
}

bool Material::operator==(const Material& material)
{
	return material.color == this->color && material.texture == this->texture;
}