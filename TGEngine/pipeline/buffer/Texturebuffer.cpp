#include "Texturebuffer.hpp"

std::vector<Texture*> texture_buffers;
std::vector<Material> materials;
Descriptor texture_descriptor;
VkSampler tex_image_sampler;
uint32_t tex_array_index = 0;
std::vector<RenderOffsets> render_offset;

using namespace nio;

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

	createPipeline(shaders_for_tex, 2);
	this->pipeline_index = last_size;

	camera_uniform.descriptor.binding = 0;
	camera_uniform.updateDescriptor();

	light_buffer.descriptor.binding = 1;
	light_buffer.updateDescriptor();

	texture_descriptor.updateImageInfo(tex_image_sampler, this->texture->image_view);
}

bool Material::operator==(const Material & material)
{
	return material.color == this->color && material.texture == this->texture;
}

void createTexture(Texture* tex) {
	texture_buffers.push_back(tex);
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
		1,
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

		if (ptr->texture_path) {
			File file = open(ptr->texture_path, "rb");
			ptr->image_data = stbi_load_from_file(file, &ptr->width, &ptr->height, &ptr->channel, STBI_rgb_alpha);
		}

		vlib_image_create_info.format = ptr->image_format;
		vlib_image_create_info.extent.width = ptr->width;
		vlib_image_create_info.extent.height = ptr->height;
		if (ptr->miplevels == AUTO_MIPMAP) 
			ptr->miplevels = vlib_image_create_info.mipLevels = (uint32_t)floor(log2(math::u_max(vlib_image_create_info.extent.width, math::u_max(vlib_image_create_info.extent.height, vlib_image_create_info.extent.depth)))) + 1;
		else 
			vlib_image_create_info.mipLevels = ptr->miplevels;
		last_result = vkCreateImage(device, &vlib_image_create_info, nullptr, &ptr->image);
		HANDEL(last_result)

		vkGetImageMemoryRequirements(device, ptr->image, &ptr->requierments);

		vlib_buffer_memory_allocate_info.allocationSize = ptr->requierments.size;
		vlib_buffer_memory_allocate_info.memoryTypeIndex = vlib_device_local_memory_index;
		last_result = vkAllocateMemory(device, &vlib_buffer_memory_allocate_info, nullptr, &ptr->d_memory);
		HANDEL(last_result)

		last_result = vkBindImageMemory(device, ptr->image, ptr->d_memory, 0);
		HANDEL(last_result)

		vlib_buffer_create_info.size = ptr->width * ptr->height * 4;
		last_result = vkCreateBuffer(device, &vlib_buffer_create_info, nullptr, &ptr->buffer);
		HANDEL(last_result)

		vkGetBufferMemoryRequirements(device, ptr->buffer, &ptr->buffer_requierments);

		vlib_buffer_memory_allocate_info.allocationSize = ptr->buffer_requierments.size;
		vlib_buffer_memory_allocate_info.memoryTypeIndex = vlib_device_host_visible_coherent_index;
		last_result = vkAllocateMemory(device, &vlib_buffer_memory_allocate_info, nullptr, &ptr->buffer_memory);
		HANDEL(last_result)

		last_result = vkBindBufferMemory(device, ptr->buffer, ptr->buffer_memory, 0);
		HANDEL(last_result)

		last_result = vkMapMemory(device, ptr->buffer_memory, 0, ptr->width * ptr->height * 4, 0, &ptr->memory);
		HANDEL(last_result);

		memcpy(ptr->memory, ptr->image_data, ptr->width * ptr->height * 4);
		vkUnmapMemory(device, ptr->buffer_memory);

		if (ptr->texture_path) {
			stbi_image_free(ptr->image_data);
		}
		else {
			delete[] ptr->image_data;
		}

		vlib_image_view_create_info.format = ptr->image_format;
		vlib_image_view_create_info.image = ptr->image;
		last_result = vkCreateImageView(device, &vlib_image_view_create_info, nullptr, &ptr->image_view);
		HANDEL(last_result)
	}
}

void addTextures() {
}

void destroyBufferofTexture(Texture* tex) {
	vkFreeMemory(device, tex->buffer_memory, nullptr);
	vkDestroyBuffer(device, tex->buffer, nullptr);
}


void destroyTexture(Texture* tex) {
	vkDestroyImageView(device, tex->image_view, nullptr);
	vkFreeMemory(device, tex->d_memory, nullptr);
	vkDestroyImage(device, tex->image, nullptr);
}

void destroyAllTextures() {
	last_result = vkDeviceWaitIdle(device);
	HANDEL(last_result)

	vkDestroySampler(device, tex_image_sampler, nullptr);
	for each(Texture* tex in texture_buffers) {
		destroyTexture(tex);
	}
}