#include "UniformBuffer.hpp"

namespace tge::buf {

	BufferObject buffers;

	void initUniformBuffers() {
		BufferInputInfo bufferInputInfo;
		bufferInputInfo.flags = VK_SHADER_STAGE_VERTEX_BIT;
		bufferInputInfo.size = sizeof(glm::mat4);
		bufferInputInfo.memoryIndex = vlibDeviceHostVisibleCoherentIndex;
		bufferInputInfo.bufferUsageFlag = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

		createBuffers(&bufferInputInfo, 1, &buffers);

		VkDescriptorBufferInfo infoTransform;
		infoTransform.buffer = buffers.buffer;
		infoTransform.offset = 0;
		infoTransform.range = bufferInputInfo.size;

		VkWriteDescriptorSet writeDescriptorSet;
		writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSet.pNext = nullptr;
		writeDescriptorSet.dstSet = mainDescriptorSet;
		writeDescriptorSet.dstBinding = 0;
		writeDescriptorSet.dstArrayElement = 0;
		writeDescriptorSet.descriptorCount = 1;
		writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writeDescriptorSet.pImageInfo = nullptr;
		writeDescriptorSet.pBufferInfo = &infoTransform;
		writeDescriptorSet.pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(device, 1, &writeDescriptorSet, 0, nullptr);
	}

	void fillUniformBuffer(uint32_t uniformBufferIndex, void* data, VkDeviceSize size, VkDeviceSize offset) {
		VkDeviceMemory memory = buffers.memory;
		void* dstPtr;
		vkMapMemory(device, memory, offset, size, 0, &dstPtr);
		memcpy(dstPtr, data, size);
		vkUnmapMemory(device, memory);
	}

}
