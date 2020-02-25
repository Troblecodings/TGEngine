#include "UniformBuffer.hpp"

namespace tge::buf {

	BufferObject buffers[2];

	void initUniformBuffers() {
		BufferInputInfo bufferInputInfo[2];
		bufferInputInfo[0].flags = VK_SHADER_STAGE_VERTEX_BIT;
		bufferInputInfo[0].size = sizeof(glm::mat4);
		bufferInputInfo[0].memoryIndex = vlibDeviceHostVisibleCoherentIndex;
		bufferInputInfo[0].bufferUsageFlag = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

		bufferInputInfo[1].flags = VK_SHADER_STAGE_VERTEX_BIT;
		bufferInputInfo[1].size = sizeof(glm::mat4);
		bufferInputInfo[1].memoryIndex = vlibDeviceHostVisibleCoherentIndex;
		bufferInputInfo[1].bufferUsageFlag = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

		// TODO only use one device memory
		createBuffers(bufferInputInfo, 2, buffers);

		VkDescriptorBufferInfo infoTransform[2];
		infoTransform[0].buffer = buffers[0].buffer;
		infoTransform[0].offset = 0;
		infoTransform[0].range = bufferInputInfo[0].size;

		infoTransform[1].buffer = buffers[1].buffer;
		infoTransform[1].offset = 0;
		infoTransform[1].range = bufferInputInfo[1].size;

		VkWriteDescriptorSet writeDescriptorSet[2];
		writeDescriptorSet[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSet[0].pNext = nullptr;
		writeDescriptorSet[0].dstSet = mainDescriptorSets[1];
		writeDescriptorSet[0].dstBinding = 0;
		writeDescriptorSet[0].dstArrayElement = 0;
		writeDescriptorSet[0].descriptorCount = 1;
		writeDescriptorSet[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writeDescriptorSet[0].pImageInfo = nullptr;
		writeDescriptorSet[0].pBufferInfo = &infoTransform[0];
		writeDescriptorSet[0].pTexelBufferView = nullptr;

		writeDescriptorSet[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSet[1].pNext = nullptr;
		writeDescriptorSet[1].dstSet = mainDescriptorSets[2];
		writeDescriptorSet[1].dstBinding = 0;
		writeDescriptorSet[1].dstArrayElement = 0;
		writeDescriptorSet[1].descriptorCount = 1;
		writeDescriptorSet[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writeDescriptorSet[1].pImageInfo = nullptr;
		writeDescriptorSet[1].pBufferInfo = &infoTransform[1];
		writeDescriptorSet[1].pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(device, 2, writeDescriptorSet, 0, nullptr);
	}

	void fillUniformBuffer(uint32_t uniformBufferIndex, void* data, VkDeviceSize size, VkDeviceSize offset) {
		VkDeviceMemory memory = buffers[uniformBufferIndex].memory;
		void* dstPtr;
		vkMapMemory(device, memory, offset, size, 0, &dstPtr);
		memcpy(dstPtr, data, size);
		vkUnmapMemory(device, memory);
	}

}
