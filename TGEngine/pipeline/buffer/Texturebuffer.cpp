#include "Texturebuffer.hpp"
#include "../CommandBuffer.hpp"

namespace tge::tex {

	void createTextures(TextureLoaded* input, uint32_t size, TextureOutput* output)
	{
		// TODO default format checks

		VkDescriptorImageInfo* imagedesc = new VkDescriptorImageInfo[size];

		VkBuffer* bufferlist = new VkBuffer[size];
		VkDeviceMemory* memorylist = new VkDeviceMemory[size];

		VkImageMemoryBarrier* entrymemorybarriers = new VkImageMemoryBarrier[size];

		for (uint32_t i = 0; i < size; i++)
		{
			// Todo do Vulkan stuff
			TextureLoaded* tex = &input[i];
			TextureOutput* out = &output[i];

			// TODO general validation checks for image creation
			// Hold my beer! https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#resources-image-creation-limits
			VkImageCreateInfo imageCreateInfo;
			imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageCreateInfo.pNext = nullptr;
			imageCreateInfo.flags = 0;
			imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
			imageCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM; // TODO format checks this is optimistic
			imageCreateInfo.extent = { (uint32_t)tex->x, (uint32_t)tex->y, 1 };
			imageCreateInfo.mipLevels = 1; // TODO Miplevel selection. TextureIn?
			imageCreateInfo.arrayLayers = 1; // TODO layered textures. TextureIn?
			imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL; // Why would one use linear? Maybe take a look
			imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			imageCreateInfo.queueFamilyIndexCount = 0;
			imageCreateInfo.pQueueFamilyIndices = nullptr;
			imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

			lastResult = vkCreateImage(device, &imageCreateInfo, nullptr, &out->image);
			CHECKFAIL;

			VkMemoryRequirements requirements;
			vkGetImageMemoryRequirements(device, out->image, &requirements);

			VkMemoryAllocateInfo memoryAllocateInfo;
			memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			memoryAllocateInfo.pNext = nullptr;
			memoryAllocateInfo.allocationSize = requirements.size;
			memoryAllocateInfo.memoryTypeIndex = vlibDeviceLocalMemoryIndex; // Goofy

			lastResult = vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &out->imagememory);
			CHECKFAIL;

			lastResult = vkBindImageMemory(device, out->image, out->imagememory, 0);
			CHECKFAIL;

			VkBufferCreateInfo bufferCreateInfo;
			bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCreateInfo.pNext = nullptr;
			bufferCreateInfo.flags = 0;
			bufferCreateInfo.size = 4 * tex->x * tex->y;
			bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			bufferCreateInfo.queueFamilyIndexCount = 0;
			bufferCreateInfo.pQueueFamilyIndices = nullptr;

			lastResult = vkCreateBuffer(device, &bufferCreateInfo, nullptr, &bufferlist[i]);
			CHECKFAIL;

			vkGetBufferMemoryRequirements(device, bufferlist[i], &requirements);

			memoryAllocateInfo.allocationSize = requirements.size;
			memoryAllocateInfo.memoryTypeIndex = vlibDeviceHostVisibleCoherentIndex; // Goofy

			lastResult = vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &memorylist[i]);
			CHECKFAIL;

			lastResult = vkBindBufferMemory(device, bufferlist[i], memorylist[i], 0);
			CHECKFAIL;

			void* memory;
			uint32_t tmp_size = bufferCreateInfo.size;
			lastResult = vkMapMemory(device, memorylist[i], 0, tmp_size, 0, &memory);
			CHECKFAIL;
			memcpy(memory, tex->data, tmp_size);
			vkUnmapMemory(device, memorylist[i]);
			delete[] tex->data;

			VkImageViewCreateInfo imageViewCreateInfo;
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.pNext = nullptr;
			imageViewCreateInfo.flags = 0;
			imageViewCreateInfo.image = out->image;
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

			lastResult = vkCreateImageView(device, &imageViewCreateInfo, nullptr, &out->view);
			CHECKFAIL;

			imagedesc[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imagedesc[i].imageView = out->view;
			imagedesc[i].sampler = tex->sampler;

			entrymemorybarriers[i].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			entrymemorybarriers[i].pNext = nullptr;
			entrymemorybarriers[i].srcAccessMask = 0;
			entrymemorybarriers[i].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			entrymemorybarriers[i].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			entrymemorybarriers[i].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			entrymemorybarriers[i].srcQueueFamilyIndex = 0;
			entrymemorybarriers[i].dstQueueFamilyIndex = 0;
			entrymemorybarriers[i].image = out->image;
			entrymemorybarriers[i].subresourceRange = imageViewCreateInfo.subresourceRange;
		}

		startSingleTimeCommand();
		vkCmdPipelineBarrier(SINGELTIME_COMMAND_BUFFER, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, size, entrymemorybarriers);
		for (uint32_t i = 0; i < size; i++)
		{
			VkBufferImageCopy copy = vlibBufferImageCopy;
			copy.imageExtent.width = input[i].x;
			copy.imageExtent.height = input[i].y;
			vkCmdCopyBufferToImage(SINGELTIME_COMMAND_BUFFER, bufferlist[i], output[i].image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);
		}
		endSingleTimeCommand();

		for (uint32_t i = 0; i < size; i++)
		{
			vkFreeMemory(device, memorylist[i], nullptr);
			vkDestroyBuffer(device, bufferlist[i], nullptr);
		}

		VkWriteDescriptorSet descwrite;
		descwrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descwrite.pNext = nullptr;
		descwrite.dstBinding = 0;
		descwrite.pImageInfo = imagedesc;
		descwrite.descriptorCount = size;
		descwrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descwrite.dstArrayElement = 0;
		descwrite.pTexelBufferView = nullptr;
		descwrite.pBufferInfo = nullptr;
		// TODO set descriptor set
		vkUpdateDescriptorSets(device, 1, &descwrite, 0, nullptr);
	}

	void loadTextures(File file, ResourceDescriptor* input, uint32_t size, TextureLoaded* loaded)
	{
		TGE_GET_RESOURCE(
			TextureLoaded * out = &loaded[i];
		    out->data = stbi_load_from_memory(resbuffer, tex->size, &out->x, &out->y, &out->comp, STBI_rgb_alpha);
		)
	}

	void loadSampler(File file, ResourceDescriptor* input, uint32_t size, SamplerLoaded* loaded)
	{
		TGE_GET_RESOURCE(
			memcpy(loaded, resbuffer, input->size);
		)
	}

	void createSampler(SamplerLoaded* input, uint32_t size, VkSampler* sampler)
	{
		VkSamplerCreateInfo samplerCreateInfo;
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.pNext = 0;
		samplerCreateInfo.flags = 0;
		samplerCreateInfo.magFilter = input->filter;
		samplerCreateInfo.minFilter = input->filter;
		samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST; // TODO MipMaping
		samplerCreateInfo.addressModeU = input->uSamplerMode;
		samplerCreateInfo.addressModeV = input->vSamplerMode;
		samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.mipLodBias = 0;
		samplerCreateInfo.anisotropyEnable = input->anisotropyFiltering > 0;
		samplerCreateInfo.maxAnisotropy = input->anisotropyFiltering;
		samplerCreateInfo.compareEnable = VK_FALSE;
		samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
		samplerCreateInfo.minLod = 0;
		samplerCreateInfo.maxLod = 1; // TODO Lod
		samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK; // Black int color as default border color
		samplerCreateInfo.unnormalizedCoordinates = VK_TRUE;

		lastResult = vkCreateSampler(device, &samplerCreateInfo, nullptr, sampler);
		CHECKFAIL;
	}

}