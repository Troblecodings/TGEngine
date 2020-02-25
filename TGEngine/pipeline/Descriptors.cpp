#include "Descriptors.hpp"

VkPipelineLayout pipelineLayout;
VkDescriptorSet mainDescriptorSet[2];

void initDescriptors() {
	VkDescriptorPoolSize* sizes = new VkDescriptorPoolSize[3];
	sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	sizes[0].descriptorCount = 2;

	sizes[1].type = VK_DESCRIPTOR_TYPE_SAMPLER;
	sizes[1].descriptorCount = 2;

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

	VkDescriptorSetLayoutBinding descriptorSetLayoutBindingTransform;
	descriptorSetLayoutBindingTransform.binding = 0;
	descriptorSetLayoutBindingTransform.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorSetLayoutBindingTransform.descriptorCount = 1;
	descriptorSetLayoutBindingTransform.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	descriptorSetLayoutBindingTransform.pImmutableSamplers = VK_NULL_HANDLE;

	VkDescriptorSetLayoutBinding descriptorSetLayoutBindingTextures[2];
	descriptorSetLayoutBindingTextures[0].binding = 1;
	descriptorSetLayoutBindingTextures[0].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
	descriptorSetLayoutBindingTextures[0].descriptorCount = 1;
	descriptorSetLayoutBindingTextures[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	descriptorSetLayoutBindingTextures[0].pImmutableSamplers = VK_NULL_HANDLE;

	descriptorSetLayoutBindingTextures[1].binding = 2;
	descriptorSetLayoutBindingTextures[1].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	descriptorSetLayoutBindingTextures[1].descriptorCount = 2048;
	descriptorSetLayoutBindingTextures[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	descriptorSetLayoutBindingTextures[1].pImmutableSamplers = VK_NULL_HANDLE;

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo[2];
	descriptorSetLayoutCreateInfo[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo[0].pNext = nullptr;
	descriptorSetLayoutCreateInfo[0].flags = 0;
	descriptorSetLayoutCreateInfo[0].bindingCount = 1;
	descriptorSetLayoutCreateInfo[0].pBindings = &descriptorSetLayoutBindingTransform;

	descriptorSetLayoutCreateInfo[1].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo[1].pNext = nullptr;
	descriptorSetLayoutCreateInfo[1].flags = 0;
	descriptorSetLayoutCreateInfo[1].bindingCount = 2;
	descriptorSetLayoutCreateInfo[1].pBindings = descriptorSetLayoutBindingTextures;

	VkDescriptorSetLayout descriptorSetLayout[2];
	CHECKFAIL(vkCreateDescriptorSetLayout(device, descriptorSetLayoutCreateInfo, nullptr, descriptorSetLayout));

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
	pipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayout;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 2;
	pipelineLayoutCreateInfo.pPushConstantRanges = pushConstantRanges;

	CHECKFAIL(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout));

	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.pNext = nullptr;
	descriptorSetAllocateInfo.descriptorPool = descriptorPool;
	descriptorSetAllocateInfo.descriptorSetCount = 3;
	descriptorSetAllocateInfo.pSetLayouts = new VkDescriptorSetLayout[]{ descriptorSetLayout[0], descriptorSetLayout[0], descriptorSetLayout[1] };

	CHECKFAIL(vkAllocateDescriptorSets(device, &descriptorSetAllocateInfo, mainDescriptorSet));
}
