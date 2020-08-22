#include "Memory.hpp"

namespace tge::buf {

	void createBuffers(BufferInputInfo* inputInfo, uint32_t size, BufferObject* ubo) {
		for (uint32_t i = 0; i < size; i++) {
			BufferInputInfo uboInputInfo = inputInfo[i];

			VkBufferCreateInfo bufferCreateInfo;
			bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCreateInfo.pNext = nullptr;
			bufferCreateInfo.flags = 0;
			bufferCreateInfo.size = uboInputInfo.size;
			bufferCreateInfo.usage = uboInputInfo.bufferUsageFlag;
			bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			bufferCreateInfo.queueFamilyIndexCount = 0;
			bufferCreateInfo.pQueueFamilyIndices = nullptr;

			CHECKFAIL(vkCreateBuffer(device, &bufferCreateInfo, nullptr, &ubo[i].buffer));

			VkMemoryRequirements memoryRequirements;
			vkGetBufferMemoryRequirements(device, ubo[i].buffer, &memoryRequirements);

			VkMemoryAllocateInfo memoryAllocateInfo;
			memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			memoryAllocateInfo.pNext = nullptr;
			memoryAllocateInfo.allocationSize = memoryRequirements.size;
			memoryAllocateInfo.memoryTypeIndex = uboInputInfo.memoryIndex;
			CHECKFAIL(vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &ubo[i].memory));

			CHECKFAIL(vkBindBufferMemory(device, ubo[i].buffer, ubo[i].memory, 0));
		}
	}

	void destroyBuffers(BufferObject* ubos, uint32_t size) {
		for (uint32_t i = 0; i < size; i++) {
			BufferObject obj = ubos[i];
			if (obj.memory == VK_NULL_HANDLE)
				continue;
			vkDestroyBuffer(device, obj.buffer, nullptr);
			vkFreeMemory(device, obj.memory, nullptr);
		}
	}

}