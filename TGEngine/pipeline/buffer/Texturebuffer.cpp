#include "Texturebuffer.hpp"
#include "Memory.hpp"

namespace tge {
	namespace tex {

		std::vector<Texture*> textures;
		Descriptor textureDescriptor;
		VkSampler defaultImageSampler;
		VkFormat defaultImageFormat;

		using namespace tge::nio;

		Texture::Texture(const char* textureName) : textureName(textureName) {
			this->sampler = defaultImageSampler;
			ASSERT_NONE_NULL_DB(textureName, "File name null", TG_ERR_DB_NULLPTR)
				ASSERT_NONE_NULL_DB((*textureName != 0), "File name is empty", TG_ERR_DB_NULLPTR)
		}

		Texture::Texture(int width, int height) {
			this->width = width;
			this->height = height;
			this->sampler = defaultImageSampler;
		}

		void Texture::initTexture() {
			// Load texture if not build in (textureName == nulptr)
			if(this->textureName) {
				File file = open(const_cast<char*> (this->textureName), "rb");
				this->imageData = stbi_load_from_file(file, &this->width, &this->height, &this->channel, STBI_rgb_alpha);
			}

			// Imageview create parameters
			vlibImageCreateInfo.extent.width = this->width;
			vlibImageCreateInfo.extent.height = this->height;

			// Buffer size
			vlibBufferCreateInfo.size = (uint32_t)this->width * (uint32_t)this->height * (uint32_t)4;

			// Mipmaplevels
			if(this->miplevels == AUTO_MIPMAP)
				this->miplevels = (uint32_t)floor(log2(math::u_max(vlibImageCreateInfo.extent.width, math::u_max(vlibImageCreateInfo.extent.height, vlibImageCreateInfo.extent.depth)))) + 1;
			vlibImageCreateInfo.mipLevels = this->miplevels;

			this->initVulkan();
			void* memory = this->map(this->width + this->height * 4);
			memcpy(memory, this->imageData, vlibBufferCreateInfo.size);
			this->unmap();

			// Destroy unneeded resources
			if(this->textureName) {
				stbi_image_free(const_cast<unsigned char*>(this->imageData));
			}
		}

		void Texture::load(VkCommandBuffer buffer) {
			vlibImageMemoryBarrier.subresourceRange.baseMipLevel = 0;
			vlibImageMemoryBarrier.subresourceRange.levelCount = this->miplevels;

			vlibBufferImageCopy.imageExtent.width = this->width;
			vlibBufferImageCopy.imageExtent.height = this->height;

			this->queueLoading(buffer);
			this->generateMipmaps(buffer);
		}

		int Texture::getWidth() {
			return this->width;
		}

		int Texture::getHeight() {
			return this->height;
		}

		void Texture::updateDescriptor() {
			textureDescriptor.updateImageInfo(this->sampler, this->imageView);
		}

		void Texture::queueLoading(VkCommandBuffer buffer) {
			// Copys the buffer to the image
			ADD_IMAGE_MEMORY_BARRIER(buffer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, this->image, 0, VK_ACCESS_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT)

				vkCmdCopyBufferToImage(
				buffer,
				this->buffer,
				this->image,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&vlibBufferImageCopy
				);
		}

		void Texture::generateMipmaps(VkCommandBuffer buffer) {
			uint32_t mipwidth = this->width,
				mipheight = this->height,
				mipmapLevels = vlibImageMemoryBarrier.subresourceRange.levelCount;

			vlibImageMemoryBarrier.subresourceRange.levelCount = 1;

			for(uint32_t i = 1; i < mipmapLevels; i++) {
				vlibImageMemoryBarrier.subresourceRange.baseMipLevel = i - 1;
				ADD_IMAGE_MEMORY_BARRIER(buffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, this->image, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT)

					vlibImageBlit.srcSubresource.mipLevel = i - 1;
				vlibImageBlit.dstSubresource.mipLevel = i;
				vlibImageBlit.srcOffsets[1].x = mipwidth;
				vlibImageBlit.srcOffsets[1].y = mipheight;
				vlibImageBlit.dstOffsets[1].x = mipwidth > 1 ? mipwidth / (uint32_t)2 : 1;
				vlibImageBlit.dstOffsets[1].y = mipheight > 1 ? mipheight / (uint32_t)2 : 1;
				vkCmdBlitImage(buffer, this->image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, this->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &vlibImageBlit, VK_FILTER_LINEAR);

				ADD_IMAGE_MEMORY_BARRIER(buffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, this->image, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT)

					if(mipwidth > 1) mipwidth /= 2;
				if(mipheight > 1) mipheight /= 2;
			}

			if(mipmapLevels > 1) {
				vlibImageMemoryBarrier.subresourceRange.baseMipLevel = mipmapLevels - 1;
			}

			ADD_IMAGE_MEMORY_BARRIER(buffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, this->image, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT)
		}

		void initAllTextures() {
			// Querries formats
			for(VkFormat format : { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_A2B10G10R10_UNORM_PACK32, VK_FORMAT_B10G11R11_UFLOAT_PACK32}) {
				VkFormatProperties prop;
				vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &prop);
				if((prop.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) == VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) {
					defaultImageFormat = format;
					break;
				}
			}

			// Creates default sampler
			createSampler(&defaultImageSampler);

			textureDescriptor = Descriptor(VK_SHADER_STAGE_FRAGMENT_BIT, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2);

			vlibImageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
			vlibImageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			vlibImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			// TODO Look into this
			vlibImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_B;
			vlibImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_R;
			//

			// Get default 
			VkImageFormatProperties imageFormatProperties;
			lastResult = vkGetPhysicalDeviceImageFormatProperties(physicalDevice, vlibImageCreateInfo.format,
				vlibImageCreateInfo.imageType, vlibImageCreateInfo.tiling, vlibImageCreateInfo.usage, vlibImageCreateInfo.flags, &imageFormatProperties);
			HANDEL(lastResult)

			// TODO implement checks
		}

		void createSampler(Sampler* sampler) {
			lastResult = vkCreateSampler(device, &vlibSamplerCreateInfo, nullptr, sampler);
			HANDEL(lastResult)
		}

		void destroyAllTextures() {
			lastResult = vkDeviceWaitIdle(device);
			HANDEL(lastResult)

				vkDestroySampler(device, defaultImageSampler, nullptr);
			for each(Texture* tex in textures) {
				tex->destroy();
			}
		}

		void Texture::initVulkan() {
			vlibImageCreateInfo.format = defaultImageFormat;

			// Image
			lastResult = vkCreateImage(device, &vlibImageCreateInfo, nullptr, &this->image);
			HANDEL(lastResult)

				QUERRY_IMAGE_REQUIREMENTS(this->image, vlibDeviceLocalMemoryIndex)
				lastResult = vkAllocateMemory(device, &vlibBufferMemoryAllocateInfo, nullptr, &this->imageMemory);
			HANDEL(lastResult)

				lastResult = vkBindImageMemory(device, this->image, this->imageMemory, 0);
			HANDEL(lastResult)

				// Buffer
				lastResult = vkCreateBuffer(device, &vlibBufferCreateInfo, nullptr, &this->buffer);
			HANDEL(lastResult)

				QUERRY_BUFFER_REQUIREMENTS(this->buffer, vlibDeviceHostVisibleCoherentIndex)
				lastResult = vkAllocateMemory(device, &vlibBufferMemoryAllocateInfo, nullptr, &bufferMemory);
			HANDEL(lastResult)

				lastResult = vkBindBufferMemory(device, this->buffer, this->bufferMemory, 0);
			HANDEL(lastResult)

				// Imageview
				vlibImageViewCreateInfo.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
			vlibImageViewCreateInfo.image = this->image;
			lastResult = vkCreateImageView(device, &vlibImageViewCreateInfo, nullptr, &this->imageView);
			HANDEL(lastResult)
		}

		void* Texture::map(uint32_t size) {
			void* memory;
			lastResult = vkMapMemory(device, this->bufferMemory, 0, size, 0, &memory);
			HANDEL(lastResult);
			return memory;
		}

		void Texture::unmap() {
			vkUnmapMemory(device, this->bufferMemory);
		}

		void Texture::dispose() {
			vkFreeMemory(device, this->bufferMemory, nullptr);
			vkDestroyBuffer(device, this->buffer, nullptr);
		}

		void Texture::destroy() {
			vkDestroyImageView(device, this->imageView, nullptr);
			vkFreeMemory(device, this->imageMemory, nullptr);
			vkDestroyImage(device, this->image, nullptr);
		}

	}
}