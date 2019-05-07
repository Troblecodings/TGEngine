#include "Texturebuffer.hpp"

std::vector<Texture*> texture_buffers;
std::vector<Material> materials;
Descriptor texture_descriptor;
VkSampler tex_image_sampler;
std::vector<RenderOffsets> render_offset;

using namespace tge::nio;

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
	
	this->descriptor_index = texture_descriptor.descriptorset = light_buffer.descriptor.descriptorset = camera_uniform.descriptor.descriptorset = createDescriptorSet(this->layout_index);

	createPipeline(shaders_for_tex, 2, this->layout_index);
	this->pipeline_index = last_size;

	camera_uniform.descriptor.binding = 0;
	camera_uniform.updateDescriptor();

	light_buffer.descriptor.binding = 1;
	light_buffer.updateDescriptor();

	texture_descriptor.updateImageInfo(tex_image_sampler, this->texture->vulkanTexture );
}

void Material::destroy()
{
	destroyDesctiptorSet(this->descriptor_index);
	//destroyDesctiptorLayout(this->layout_index);
	destroyPipeline(this->pipeline_index);
	destroyPipelineLayout(this->layout_index);
}

bool Material::operator==(const Material & material)
{
	return material.color == this->color && material.texture == this->texture;
}

Texture::Texture() {
	texture_buffers.push_back(this);
}

void Texture::initTexture()
{
	// Load texture if not build in (textureName == nulptr)
	if (this->textureName) {
		File file = open(this->textureName, "rb");
		this->imageData = stbi_load_from_file(file, &this->width, &this->height, &this->channel, STBI_rgb_alpha);
	}

	// Imageview create parameters
	vlib_image_create_info.extent.width = this->width;
	vlib_image_create_info.extent.height = this->height;

	// Mipmaplevels
	if (this->miplevels == AUTO_MIPMAP)
		this->miplevels = (uint32_t)floor(log2(math::u_max(vlib_image_create_info.extent.width, math::u_max(vlib_image_create_info.extent.height, vlib_image_create_info.extent.depth)))) + 1;
	vlib_image_create_info.mipLevels = this->miplevels;

	this->vulkanTexture.initVulkan();
	void* memory = this->vulkanTexture.map(this->width + this->height * 4);
	memcpy(memory, this->imageData, this->width * this->height * 4);

	// Destroy unneeded resources
	if (this->textureName) {
		stbi_image_free(this->imageData);
	}
	else {
		delete[] this->imageData;
	}
}

void initAllTextures() {
	VkSamplerCreateInfo sampler_create_info = {
		VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		nullptr,
		0,
		VK_FILTER_LINEAR,
		VK_FILTER_LINEAR,
		VK_SAMPLER_MIPMAP_MODE_LINEAR,
		VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		0,
		VK_TRUE,
		16.0f,
		VK_TRUE,
		VK_COMPARE_OP_ALWAYS,
		0,
		10,
		VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK,
		VK_FALSE
	};
	last_result = vkCreateSampler(device, &sampler_create_info, nullptr, &tex_image_sampler);
	HANDEL(last_result)

	texture_descriptor = Descriptor(VK_SHADER_STAGE_FRAGMENT_BIT, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2);

	vlib_image_create_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	vlib_image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
	vlib_image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

	for each(Texture* ptr in texture_buffers) {
		ptr->initTexture();
	}
}

void destroyAllTextures() {
	last_result = vkDeviceWaitIdle(device);
	HANDEL(last_result)

	vkDestroySampler(device, tex_image_sampler, nullptr);
	for each(Texture* tex in texture_buffers) {
		tex->vulkanTexture.destroy();
	}
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

void VulkanTexture::initVulkan()
{
	vlib_image_create_info.format = this->imageFormat; // TODO Auto querry

	// TODO FIX THIS MESS
	// TODO AUTO QUERRY
	VkImageFormatProperties imageFormatProperties;
	last_result = vkGetPhysicalDeviceImageFormatProperties(used_physical_device, vlib_image_create_info.format,
	vlib_image_create_info.imageType, vlib_image_create_info.tiling, vlib_image_create_info.usage, vlib_image_create_info.flags, &imageFormatProperties);
	HANDEL(last_result)
	//
	
	// Image
	last_result = vkCreateImage(device, &vlib_image_create_info, nullptr, &this->image);
	HANDEL(last_result)

	QUERRY_IMAGE_REQUIREMENTS(this->image, vlib_device_local_memory_index)
	last_result = vkAllocateMemory(device, &vlib_buffer_memory_allocate_info, nullptr, &this->imageMemory);
	HANDEL(last_result)

	last_result = vkBindImageMemory(device, this->image, this->imageMemory, 0);
	HANDEL(last_result)

	// Buffer
	last_result = vkCreateBuffer(device, &vlib_buffer_create_info, nullptr, &this->buffer);
	HANDEL(last_result)

	QUERRY_BUFFER_REQUIREMENTS(this->buffer, vlib_device_host_visible_coherent_index)
	last_result = vkAllocateMemory(device, &vlib_buffer_memory_allocate_info, nullptr, &bufferMemory);
	HANDEL(last_result)

	last_result = vkBindBufferMemory(device, this->buffer, this->bufferMemory, 0);
	HANDEL(last_result)

	// Imageview
	vlib_image_view_create_info.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	vlib_image_view_create_info.format = this->imageFormat;
	vlib_image_view_create_info.image = this->image;
	last_result = vkCreateImageView(device, &vlib_image_view_create_info, nullptr, &this->imageView);
	HANDEL(last_result)
}

void* VulkanTexture::map(uint32_t size)
{
	void* memory;
	last_result = vkMapMemory(device, this->bufferMemory, 0, size, 0, &memory);
	HANDEL(last_result);
	return memory;
}

void VulkanTexture::unmap()
{
	vkUnmapMemory(device, this->bufferMemory);
}

void VulkanTexture::dispose()
{
	vkFreeMemory(device, this->bufferMemory, nullptr);
	vkDestroyBuffer(device, this->buffer, nullptr);
}

void VulkanTexture::destroy()
{
	vkDestroyImageView(device, this->imageView, nullptr);
	vkFreeMemory(device, this->imageMemory, nullptr);
	vkDestroyImage(device, this->image, nullptr);
}
