#pragma once

#include "../Stdbase.hpp"
#include "Device.hpp"
#include "../pipeline/buffer/Memory.hpp"
#include "../vlib/VulkanDescriptor.hpp"

extern VkDescriptorPool descriptor_pool;
extern std::vector<VkDescriptorSet> descriptor_set;
extern std::vector<VkDescriptorSetLayout> descriptor_set_layouts;
extern std::vector<VkDescriptorSetLayoutBinding> descriptor_bindings;

extern uint32_t uniform_count;
extern uint32_t image_sampler_count;

class Descriptor {

public:

	INTERNAL
	SINCE(0, 0, 4)
	Descriptor() {} // Default constructor
	Descriptor(VkShaderStageFlags stage, VkDescriptorType type);
	Descriptor(VkDescriptorType type) : Descriptor(VK_SHADER_STAGE_VERTEX_BIT, type) {}

	INPUT  
	VkShaderStageFlags shader_stage;
	uint32_t count = 1;
	VkDescriptorType type;
	uint32_t buffer = 0;

	VkSampler image_sampler;
	VkImageView* image_view;
	uint32_t array_index = 0;
	size_t descriptor_set = 0;

	OUTPUT
    uint32_t binding;
	VkDescriptorImageInfo* desc_image_info;
	VkDescriptorBufferInfo buffer_info;
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
 * - Note: for VkDescriptorSetLayoutBinding  see the Vulkan docs
 *         https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkDescriptorSetLayoutBinding.html
 */
INTERNAL
SINCE(0, 0, 4)
void addDescriptorBinding(uint32_t binding, VkDescriptorType type, VkShaderStageFlags flags);

/*
 * Creates a descriptor layout
 *    -> returns the index of the descriptor
 *
 * - Note: for VkDescriptorSetLayout see the Vulkan docs 
 *         https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkDescriptorSetLayout.html
 */
INTERNAL
SINCE(0, 0, 4)
size_t createDesctiptorLayout();

/*
 * 
 */
INTERNAL
SINCE(0, 0, 4)
size_t createDescriptorSet(uint32_t layout = 0);

INTERNAL
SINCE(0, 0, 2)
void updateDescriptorSet(Descriptor* desc, uint32_t size);

INTERNAL
SINCE(0, 0, 2)
void destroyDescriptors();
