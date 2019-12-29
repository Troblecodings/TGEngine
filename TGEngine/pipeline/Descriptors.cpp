#include "Descriptors.hpp"

VkPipelineLayout pipelineLayout;
VkDescriptorSet mainDescriptorSet[2];

void initDescriptors() {
	VkDescriptorPoolSize* sizes = new VkDescriptorPoolSize[3];
	sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	sizes[0].descriptorCount = 1;

	sizes[1].type = VK_DESCRIPTOR_TYPE_SAMPLER;
	sizes[1].descriptorCount = 1;

	sizes[2].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	sizes[2].descriptorCount = 2048;

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.pNext = nullptr;
	descriptorPoolCreateInfo.flags = 0;
	descriptorPoolCreateInfo.maxSets = 10; // Todo validation checks and modifiable
	descriptorPoolCreateInfo.poolSizeCount = 3;
	descriptorPoolCreateInfo.pPoolSizes = sizes;

	VkDescriptorPool descriptorPool;
	CHECKFAIL(vkCreateDescriptorPool(device, &descriptorPoolCreateInfo, nullptr, &descriptorPool));

	VkDescriptorSetLayoutBinding descriptorSetLayoutBinding[3];
	descriptorSetLayoutBinding[0].binding = 0;
	descriptorSetLayoutBinding[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorSetLayoutBinding[0].descriptorCount = 1;
	descriptorSetLayoutBinding[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	descriptorSetLayoutBinding[0].pImmutableSamplers = VK_NULL_HANDLE;

	descriptorSetLayoutBinding[1].binding = 1;
	descriptorSetLayoutBinding[1].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
	descriptorSetLayoutBinding[1].descriptorCount = 1;
	descriptorSetLayoutBinding[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	descriptorSetLayoutBinding[1].pImmutableSamplers = VK_NULL_HANDLE;

	descriptorSetLayoutBinding[2].binding = 2;
	descriptorSetLayoutBinding[2].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	descriptorSetLayoutBinding[2].descriptorCount = 2048;
	descriptorSetLayoutBinding[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	descriptorSetLayoutBinding[2].pImmutableSamplers = VK_NULL_HANDLE;

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.pNext = nullptr;
	descriptorSetLayoutCreateInfo.flags = 0;
	descriptorSetLayoutCreateInfo.bindingCount = 3;
	descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayoutBinding;

	VkDescriptorSetLayout descriptorSetLayout;
	CHECKFAIL(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayout));

	VkPushConstantRange pushConstantRanges[2];
	pushConstantRanges[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	pushConstantRanges[0].offset = 0;
	pushConstantRanges[0].size = 64; // mat4

	pushConstantRanges[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRanges[1].offset = 64;
	pushConstantRanges[1].size = 20; // Material

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.pNext = nullptr;
	pipelineLayoutCreateInfo.flags = 0;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 2;
	pipelineLayoutCreateInfo.pPushConstantRanges = pushConstantRanges;

	CHECKFAIL(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout));

	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.pNext = nullptr;
	descriptorSetAllocateInfo.descriptorPool = descriptorPool;
	descriptorSetAllocateInfo.descriptorSetCount = 2;
	descriptorSetAllocateInfo.pSetLayouts = new VkDescriptorSetLayout[2]{ descriptorSetLayout, descriptorSetLayout };

	CHECKFAIL(vkAllocateDescriptorSets(device, &descriptorSetAllocateInfo, mainDescriptorSet));
}
