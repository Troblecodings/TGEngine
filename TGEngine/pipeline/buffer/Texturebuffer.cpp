#include "Texturebuffer.hpp"
#include "../CommandBuffer.hpp"

namespace tge::tex {

	using namespace tge::nio;

	void createTextures(TextureIn* input, uint32_t size, TextureOutput* output)
	{
		// TODO default format checks

		File resc = open("resources\Resources.tgr", "rb");

		VkDescriptorImageInfo* imagedesc = new VkDescriptorImageInfo[size];

		for (uint32_t i = 0; i < size; i++)
		{
			TextureIn tex = input[i];
			TextureOutput out = output[i];

			if (ftell(resc) != tex.offset)
				fseek(resc, tex.offset, SEEK_SET);

			stbi_uc* imgbuffer = new stbi_uc[tex.size];
			fread(imgbuffer, sizeof(char), tex.size, resc);
			stbi_uc* loaded = stbi_load_from_memory(imgbuffer, tex.size, &out.x, &out.y, &out.comp, STBI_rgb_alpha);
			delete[] imgbuffer;
			
			// Todo do Vulkan stuff

			// TODO general validation checks for image creation
			// Hold my beer! https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#resources-image-creation-limits
			VkImageCreateInfo imageCreateInfo;
			imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageCreateInfo.pNext = nullptr;
			imageCreateInfo.flags = 0;
			imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
			imageCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM; // TODO format checks this is optimistic
			imageCreateInfo.extent = { out.x, out.y, 1};
			imageCreateInfo.mipLevels = 1; // TODO Miplevel selection. TextureIn?
			imageCreateInfo.arrayLayers = 1; // TODO layered textures. TextureIn?
			imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL; // Why would one use linear? Maybe take a look
			imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT & VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			imageCreateInfo.queueFamilyIndexCount = 0;
			imageCreateInfo.pQueueFamilyIndices = nullptr;
			imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

			lastResult = vkCreateImage(device, &imageCreateInfo, nullptr, &out.image);
			CHECKFAIL;

			VkBufferCreateInfo bufferCreateInfo;
			bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCreateInfo.pNext = nullptr;
			bufferCreateInfo.flags = 0;
			bufferCreateInfo.size = 4 * out.x * out.y;
			bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			bufferCreateInfo.queueFamilyIndexCount = 0;
			bufferCreateInfo.pQueueFamilyIndices = nullptr;

			VkBuffer buffer;
			lastResult = vkCreateBuffer(device, &bufferCreateInfo, nullptr, &buffer);
			CHECKFAIL;

			VkMemoryRequirements requirements;
			vkGetBufferMemoryRequirements(device, buffer, &requirements);

			VkMemoryAllocateInfo memoryAllocateInfo;
			memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			memoryAllocateInfo.pNext = nullptr;
			memoryAllocateInfo.allocationSize = requirements.size;
			memoryAllocateInfo.memoryTypeIndex = vlibDeviceHostVisibleCoherentIndex; // Goofy
			
			VkDeviceMemory devicememory;
			lastResult = vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &devicememory);
			CHECKFAIL;

			void* memory;
			uint32_t tmp_size = bufferCreateInfo.size;
			vkMapMemory(device, devicememory, 0, tmp_size, 0, &memory);
			memcpy(memory, loaded, tmp_size);
			vkUnmapMemory(device, devicememory);
			delete[] loaded;

			startSingleTimeCommand();
			vkCmdCopyBufferToImage(SINGELTIME_COMMAND_BUFFER, buffer, out.image, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 0, nullptr);
			endSingleTimeCommand();

			vkFreeMemory(device, devicememory, nullptr);
			vkDestroyBuffer(device, buffer, nullptr);

			VkImageViewCreateInfo imageViewCreateInfo;
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.pNext = nullptr;
			imageViewCreateInfo.flags = 0;
			imageViewCreateInfo.image = out.image;
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

			lastResult = vkCreateImageView(device, &imageViewCreateInfo, nullptr, &out.view);
			CHECKFAIL;

			imagedesc[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imagedesc[i].imageView = out.view;
			imagedesc[i].sampler = tex.sampler;
		}
		fclose(resc);

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
		delete[] imagedesc;
	}

}