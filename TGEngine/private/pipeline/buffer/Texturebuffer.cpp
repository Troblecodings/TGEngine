#include "../../../public/pipeline/buffer/Texturebuffer.hpp"
#include "../../../public/pipeline/CommandBuffer.hpp"
#include "../../../public/pipeline/Descriptors.hpp"

namespace tge::tex {

	TextureDefaults defaults;

	void createTextures(TextureInputInfo* input, uint32_t size, Texture* output, uint32_t offset) {
		// TODO default format checks

		VkBuffer* bufferlist = new VkBuffer[size];
		VkDeviceMemory* memorylist = new VkDeviceMemory[size];

		VkImageMemoryBarrier* entrymemorybarriers = new VkImageMemoryBarrier[size];
		VkImageMemoryBarrier* exitmemorybarriers = new VkImageMemoryBarrier[size];
		TextureBindingInfo imagedesc[MAX_TEXTURES];

		for (uint32_t i = 0; i < size; i++) {
			// Todo do Vulkan stuff
			TextureInputInfo tex = input[i];
#if DEBUG
			if (tex.comp != 4) {
				OUT_LV_DEBUG("Invalid component count, must be 4! Texture index: " << i);
			}
#endif // DEBUG

			// TODO general validation checks for image creation
			// Hold my beer! https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#resources-image-creation-limits
			VkImageCreateInfo imageCreateInfo;
			imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageCreateInfo.pNext = nullptr;
			imageCreateInfo.flags = 0;
			imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
			imageCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
			imageCreateInfo.extent = { (uint32_t)tex.x, (uint32_t)tex.y, 1 };
			imageCreateInfo.mipLevels = 1; // TODO Miplevel selection. TextureIn?
			imageCreateInfo.arrayLayers = 1; // TODO layered textures. TextureIn?
			imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			imageCreateInfo.queueFamilyIndexCount = 0;
			imageCreateInfo.pQueueFamilyIndices = nullptr;
			imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

			CHECKFAIL(vkCreateImage(device, &imageCreateInfo, nullptr, &output[i].image));

			VkMemoryRequirements requirements;
			vkGetImageMemoryRequirements(device, output[i].image, &requirements);

			VkMemoryAllocateInfo memoryAllocateInfo;
			memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			memoryAllocateInfo.pNext = nullptr;
			memoryAllocateInfo.allocationSize = requirements.size;
			memoryAllocateInfo.memoryTypeIndex = vlibDeviceLocalMemoryIndex; // Goofy

			CHECKFAIL(vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &output[i].imagememory));

			CHECKFAIL(vkBindImageMemory(device, output[i].image, output[i].imagememory, 0));

			VkBufferCreateInfo bufferCreateInfo;
			bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCreateInfo.pNext = nullptr;
			bufferCreateInfo.flags = 0;
			bufferCreateInfo.size = 4 * tex.x * tex.y;
			bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			bufferCreateInfo.queueFamilyIndexCount = 0;
			bufferCreateInfo.pQueueFamilyIndices = nullptr;

			CHECKFAIL(vkCreateBuffer(device, &bufferCreateInfo, nullptr, &bufferlist[i]));


			vkGetBufferMemoryRequirements(device, bufferlist[i], &requirements);

			memoryAllocateInfo.allocationSize = requirements.size;
			memoryAllocateInfo.memoryTypeIndex = vlibDeviceHostVisibleCoherentIndex; // Goofy

			CHECKFAIL(vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &memorylist[i]));

			CHECKFAIL(vkBindBufferMemory(device, bufferlist[i], memorylist[i], 0));

			void* memory;
			uint32_t tmp_size = (uint32_t)bufferCreateInfo.size;
			CHECKFAIL(vkMapMemory(device, memorylist[i], 0, tmp_size, 0, &memory));
			memcpy(memory, tex.data, tmp_size);
			vkUnmapMemory(device, memorylist[i]);

			VkImageViewCreateInfo imageViewCreateInfo;
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.pNext = nullptr;
			imageViewCreateInfo.flags = 0;
			imageViewCreateInfo.image = output[i].image;
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; // What about cube maps? Checks? TextureIn?
			imageViewCreateInfo.format = imageCreateInfo.format;                         // This may change depending on format
			imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
			imageViewCreateInfo.subresourceRange = {
				VK_IMAGE_ASPECT_COLOR_BIT,
				0,
				VK_REMAINING_MIP_LEVELS,
				0,
				VK_REMAINING_ARRAY_LAYERS
			}; // Look into the subresourcerange -> tide to miplevels and mipmaps

			CHECKFAIL(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &output[i].view));;


			imagedesc[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imagedesc[i].imageView = output[i].view;
			imagedesc[i].sampler = VK_NULL_HANDLE;

			entrymemorybarriers[i].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			entrymemorybarriers[i].pNext = nullptr;
			entrymemorybarriers[i].srcAccessMask = 0;
			entrymemorybarriers[i].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			entrymemorybarriers[i].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			entrymemorybarriers[i].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			entrymemorybarriers[i].srcQueueFamilyIndex = 0;
			entrymemorybarriers[i].dstQueueFamilyIndex = 0;
			entrymemorybarriers[i].image = output[i].image;
			entrymemorybarriers[i].subresourceRange = imageViewCreateInfo.subresourceRange;

			exitmemorybarriers[i].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			exitmemorybarriers[i].pNext = nullptr;
			exitmemorybarriers[i].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			exitmemorybarriers[i].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			exitmemorybarriers[i].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			exitmemorybarriers[i].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			exitmemorybarriers[i].srcQueueFamilyIndex = 0;
			exitmemorybarriers[i].dstQueueFamilyIndex = 0;
			exitmemorybarriers[i].image = output[i].image;
			exitmemorybarriers[i].subresourceRange = imageViewCreateInfo.subresourceRange;
		}

		startSingleTimeCommand();
		vkCmdPipelineBarrier(SINGLE_TIME_COMMAND_BUFFER, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, size, entrymemorybarriers);
		for (uint32_t i = 0; i < size; i++) {
			VkBufferImageCopy copy;
			copy.bufferOffset = 0;
			copy.bufferRowLength = 0;
			copy.bufferImageHeight = 0;
			copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			copy.imageSubresource.mipLevel = 0;
			copy.imageSubresource.baseArrayLayer = 0;
			copy.imageSubresource.layerCount = 1;
			copy.imageOffset = { 0, 0, 0 };
			copy.imageExtent.width = input[i].x;
			copy.imageExtent.height = input[i].y;
			copy.imageExtent.depth = 1;
			vkCmdCopyBufferToImage(SINGLE_TIME_COMMAND_BUFFER, bufferlist[i], output[i].image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);
		}
		vkCmdPipelineBarrier(SINGLE_TIME_COMMAND_BUFFER, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, size, exitmemorybarriers);
		endSingleTimeCommand();

		for (uint32_t i = size; i < MAX_TEXTURES; i++) {
			imagedesc[i] = imagedesc[0];
		}

		for (uint32_t i = 0; i < size; i++) {
			vkFreeMemory(device, memorylist[i], nullptr);
			vkDestroyBuffer(device, bufferlist[i], nullptr);
		}

		VkWriteDescriptorSet descwrite;
		descwrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descwrite.pNext = nullptr;
		descwrite.dstSet = mainDescriptorSets[0];
		descwrite.dstBinding = 2;
		descwrite.dstArrayElement = 0;
		descwrite.descriptorCount = MAX_TEXTURES;
		descwrite.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		descwrite.pImageInfo = imagedesc;
		descwrite.pBufferInfo = nullptr;
		descwrite.pTexelBufferView = nullptr;
		vkUpdateDescriptorSets(device, 1, &descwrite, 0, nullptr);
	}

	void destroyTexture(Texture* texture, uint32_t size) {
		for (uint32_t i = 0; i < size; i++) {
			vkDestroyImageView(device, texture[i].view, nullptr);
			vkDestroyImage(device, texture[i].image, nullptr);
			vkFreeMemory(device, texture[i].imagememory, nullptr);
		}
	}

	void createSampler(SamplerInputInfo loaded, Sampler* sampler) {
		// TODO Validation checks

		VkSamplerCreateInfo samplerCreateInfo;
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.pNext = 0;
		samplerCreateInfo.flags = 0;
		samplerCreateInfo.magFilter = loaded.filterMagnification;
		samplerCreateInfo.minFilter = loaded.filterMignification;
		samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST; // TODO MipMaping
		samplerCreateInfo.addressModeU = loaded.uSamplerMode;
		samplerCreateInfo.addressModeV = loaded.vSamplerMode;
		samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.mipLodBias = 0;
		samplerCreateInfo.anisotropyEnable = defaults.anisotropyFilter > 0;
		samplerCreateInfo.maxAnisotropy = defaults.anisotropyFilter;
		samplerCreateInfo.compareEnable = VK_TRUE;
		samplerCreateInfo.compareOp = VK_COMPARE_OP_LESS;
		samplerCreateInfo.minLod = 0;
		samplerCreateInfo.maxLod = 1; // TODO Lod
		samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
		samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

		CHECKFAIL(vkCreateSampler(device, &samplerCreateInfo, nullptr, sampler));

		SamplerBindingInfo layer;
		layer.sampler = *sampler;
		layer.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		layer.imageView = VK_NULL_HANDLE;

		VkWriteDescriptorSet descwrite;
		descwrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descwrite.pNext = nullptr;
		descwrite.dstSet = mainDescriptorSets[0];
		descwrite.dstBinding = 1;
		descwrite.dstArrayElement = 0;
		descwrite.descriptorCount = 1;
		descwrite.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
		descwrite.pImageInfo = &layer;
		descwrite.pBufferInfo = nullptr;
		descwrite.pTexelBufferView = nullptr;
		vkUpdateDescriptorSets(device, 1, &descwrite, 0, nullptr);
	}

	void destroySampler(Sampler sampler) {
		vkDestroySampler(device, sampler, nullptr);
	}

}
