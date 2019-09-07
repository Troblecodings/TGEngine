#pragma once

#include "../Stdbase.hpp"
#include "Device.hpp"
#include "../pipeline/buffer/Memory.hpp"
#include "../vlib/VulkanDescriptor.hpp"

extern VkDescriptorPool descriptor_pool; // holds the descriptor pool -> see initDescriptors()
extern std::vector<VkDescriptorSet> descriptorSets; // contains the desciptorset handles from vulkan
extern std::vector<VkDescriptorSetLayout> descriptorSetLayouts; // contains the desciptorsetlayout handles from vulkan
extern std::vector<VkDescriptorSetLayoutBinding> descriptor_bindings; // contains the desciptorsetlayoutbindings from vulkan
extern std::vector<VkPipelineLayout> pipeLayouts; // contians the PipelineLayouts

extern uint32_t uniform_count; // holds the count of uniform buffer Descriptor objects
extern uint32_t image_sampler_count; // holds the count of image sampler buffer Descriptor objects

class Descriptor {

public:

	/*
	 * Initializes all values (besides default constructor)
	 * depending on type -> increases uniform_count or image_sampler_count
	 */
	INTERNAL
		SINCE(0, 0, 4)
		Descriptor() {} // Default constructor -> doing nothing
	Descriptor(VkShaderStageFlags stage, VkDescriptorType type, uint32_t binding, uint32_t descriptorset) : shaderstage(stage), type(type), binding(binding), descriptorset(descriptorset) {}
	Descriptor(VkShaderStageFlags stage, VkDescriptorType type, uint32_t binding) : Descriptor(stage, type, binding, 0) {}
	Descriptor(VkShaderStageFlags stage, VkDescriptorType type) : Descriptor(stage, type, 0) {}
	Descriptor(VkDescriptorType type) : Descriptor(VK_SHADER_STAGE_VERTEX_BIT, type) {}

	uint32_t descriptorset = 0; // the descriptor set this is updating in
	uint32_t binding = 0;  // the binding within the shader

	/*
	 * Updates the given descriptor within its descriptor set with image infos
	 *
	 * param:
	 *    sampler -> the image sampler
	 *    view -> the image view
	 *
	 * - Note: for vkUpdateDescriptorSets see the Vulkan docs
	 *         https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/vkUpdateDescriptorSets.html
	 */
	INTERNAL
		SINCE(0, 0, 4)
		void updateImageInfo(VkSampler sampler, VkImageView view);

	/*
	 * Updates the given descriptor within its descriptor set with buffer infos
	 *
	 * param:
	 *    sampler -> the image sampler
	 *    view -> the image view
	 *
	 * - Note: for vkUpdateDescriptorSets see the Vulkan docs
	 *         https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/vkUpdateDescriptorSets.html
	 */
	INTERNAL
		SINCE(0, 0, 4)
		void updateBufferInfo(uint32_t buffer, size_t size);

private:
	VkShaderStageFlags shaderstage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	/*
	 * Updates the given descriptor within its descriptor set with buffer infos
	 *
	 * - Note: for VkWriteDescriptorSet see the Vulkan docs
	 *         https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkWriteDescriptorSet.html
	 */
	INTERNAL
		SINCE(0, 0, 4)
		void update();
};

/*
 * This initialiazes the descriptor systems (Pool, Memory ...)
 * - Note: for VkDescriptorPool  see the Vulkan docs
 *         https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkDescriptorPool.html
 */
INTERNAL
SINCE(0, 0, 4)
void initDescriptors();

/*
 * Adds a descriptor binding to the shaders applied in this pipeline
 * - Note: for VkDescriptorSetLayoutBinding see the Vulkan docs
 *         https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkDescriptorSetLayoutBinding.html
 */
INTERNAL
SINCE(0, 0, 4)
void addDescriptorBinding(uint32_t binding, VkDescriptorType type, VkShaderStageFlags flags);

/*
 * Creates a descriptor layout and it's according pipeline layout
 *    -> returns the index of the descriptor
 *
 * - Note: for VkDescriptorSetLayout see the Vulkan docs
 *         https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkDescriptorSetLayout.html
 */
INTERNAL
SINCE(0, 0, 4)
uint32_t createLayouts();

/*
 * Creates Descriptor Set
 *    -> returns the index of the descriptor
 *
 * - Note: for VkDescriptorSetLayout see the Vulkan docs
 *         https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkDescriptorSet.html
 */
INTERNAL
SINCE(0, 0, 4)
uint32_t createDescriptorSet(uint32_t layout = 0);

/*
 * Destroys the DescriptorLayout given by the index
 *
 * - Note: see the Vulkan docs
 *         https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/vkDestroyDescriptorSetLayout.html
 */
INTERNAL
SINCE(0, 0, 4)
void destroyDesctiptorLayout(uint32_t layout_index);

/*
 * Destroys the DescriptorSet given by the index
 *
 * - Note: see the Vulkan docs
 *         https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/vkFreeDescriptorSets.html
 */
INTERNAL
SINCE(0, 0, 4)
void destroyDescriptorSet(uint32_t set_index);

/*
 * Destroys all descriptor resources (pool, layouts, sets, ...)
 *
 * - Note: see the Vulkan docs
 *         https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/vkDestroyDescriptorSetLayout.html
 *         https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/vkFreeDescriptorSets.html
 *         https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/vkDestroyDescriptorPool.html
 */
INTERNAL
SINCE(0, 0, 2)
void destroyDescriptors();
