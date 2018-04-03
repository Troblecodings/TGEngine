#include "Texturebuffer.hpp"

std::vector<Texture*> texture_buffers;

void createTexture(Texture* tex) {
	uint32_t size = texture_buffers.size();
	texture_buffers.resize(size + 1);
	texture_buffers[size] = tex;
}

void initAllTextures() {
	for each(Texture* ptr in texture_buffers) {
		ptr->image_data = stbi_load(ptr->texture_path, &ptr->width, &ptr->height, &ptr->channel, STBI_rgb_alpha);
		VkImageCreateInfo image_create_info = {
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			nullptr,
			0,
			VK_IMAGE_TYPE_2D,
			VK_FORMAT_R8G8B8A8_UNORM,
		    {
			    ptr->width,
			    ptr->height,
			    1
		    },
			1,
			1,
			VK_SAMPLE_COUNT_1_BIT,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			0,
			nullptr,
			VK_IMAGE_LAYOUT_UNDEFINED
		};
		last_result = vkCreateImage(device, &image_create_info, nullptr, &ptr->image);
		HANDEL(last_result)

		uint32_t index;
		for (index = 0; index < memory_properties.memoryTypeCount; index++) {
			if (memory_properties.memoryTypes[index].propertyFlags & (VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)) break;
		}
		vkGetImageMemoryRequirements(device, ptr->image, &ptr->requierments);

		VkMemoryAllocateInfo mem_alloc_info = {
			VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		    nullptr,
			ptr->requierments.size,
			index
		};
		last_result = vkAllocateMemory(device, &mem_alloc_info, nullptr, &ptr->d_memory);
		HANDEL(last_result)

		last_result = vkBindImageMemory(device, ptr->image, ptr->d_memory, 0);
		HANDEL(last_result)

		VkBufferCreateInfo buffer_create_info = {
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		    nullptr,
		    0,
			ptr->width * ptr->height * 4,
		    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		    VK_SHARING_MODE_EXCLUSIVE,
		    0,
		    nullptr
		};
		last_result = vkCreateBuffer(device, &buffer_create_info, nullptr, &ptr->buffer);
		HANDEL(last_result)

		for (index = 0; index < memory_properties.memoryTypeCount; index++) {
			if (memory_properties.memoryTypes[index].propertyFlags & (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) break;
		}
		vkGetBufferMemoryRequirements(device, ptr->buffer, &ptr->buffer_requierments);

		VkMemoryAllocateInfo buffer_mem_alloc_info = {
			VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			nullptr,
			ptr->buffer_requierments.size,
			index
		};
		last_result = vkAllocateMemory(device, &buffer_mem_alloc_info, nullptr, &ptr->buffer_memory);
		HANDEL(last_result)

		last_result = vkBindBufferMemory(device, ptr->buffer, ptr->buffer_memory, 0);
		HANDEL(last_result)

		vkMapMemory(device, ptr->buffer_memory, 0, ptr->width * ptr->height * 4, 0, &ptr->memory);
		memcpy(ptr->memory, ptr->image_data, ptr->width * ptr->height * 4);
		vkUnmapMemory(device, ptr->buffer_memory);

		stbi_image_free(ptr->image_data);
	}
}

void Texture::addBarrier(VkCommandBuffer buffer, VkImageLayout oldLayout, VkImageLayout newLayout) {
	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	VkImageMemoryBarrier image_memory_barrier = {
		VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		nullptr,
		0,
		0,
		oldLayout,
		newLayout,
		VK_QUEUE_FAMILY_IGNORED,
		VK_QUEUE_FAMILY_IGNORED,
		this->image,
	{
		VK_IMAGE_ASPECT_COLOR_BIT,
		0,
		1,
		0,
		1
	}
	};

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		image_memory_barrier.srcAccessMask = 0;
		image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}

	vkCmdPipelineBarrier(
		buffer,
		sourceStage,
		destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &image_memory_barrier
	);
}

void destroyBufferofTexture(Texture* tex) {
	vkFreeMemory(device, tex->buffer_memory, nullptr);
	vkDestroyBuffer(device, tex->buffer, nullptr);
}


void destroyTexture(Texture* tex) {
	vkFreeMemory(device, tex->d_memory, nullptr);
	vkDestroyImage(device, tex->image, nullptr);
}

void destroyAllTextures() {
	vkDeviceWaitIdle(device);
	for each(Texture* tex in texture_buffers) {
		destroyTexture(tex);
	}
}