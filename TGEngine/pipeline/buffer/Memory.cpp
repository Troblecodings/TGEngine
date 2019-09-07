#include "Memory.hpp"

std::vector<VkBuffer> buffers = {};
std::vector<VkDeviceSize> buffer_sizes = {};
std::vector<VkDeviceSize> buffer_offsets = { 0 };
VkDeviceMemory device_memory;
VkDeviceSize _impl_size;
VkMemoryRequirements lastRequirements;

uint32_t addBuffer(VkBuffer buffer) {
	vkGetBufferMemoryRequirements(device, buffer, &lastRequirements);

	_impl_size += lastRequirements.size;

	size_t csize = buffer_offsets.size();
	buffer_offsets.resize(csize + 1);
	buffer_offsets[csize] = buffer_offsets[csize - 1] + lastRequirements.size;

	csize = buffer_sizes.size();
	buffer_sizes.resize(csize + 1);
	buffer_sizes[csize] = lastRequirements.size;

	buffers.resize(csize + 1);
	buffers[csize] = buffer;
	return (uint32_t)csize;
}

void allocateAllBuffers() {
	vlibBufferMemoryAllocateInfo.allocationSize = _impl_size;
	vlibBufferMemoryAllocateInfo.memoryTypeIndex = vlibDeviceHostVisibleCoherentIndex;
	lastResult = vkAllocateMemory(device, &vlibBufferMemoryAllocateInfo, nullptr, &device_memory);
	HANDEL(lastResult)

		for(size_t i = 0; i < buffers.size(); i++) {
			lastResult = vkBindBufferMemory(device, buffers[i], device_memory, buffer_offsets[i]);
			HANDEL(lastResult)
		}
}

void mapMemory(uint32_t buffer_index, void** mapped_memory, uint32_t size, uint32_t offset) {
	lastResult = vkMapMemory(device, device_memory, buffer_offsets[buffer_index] + offset, size == 0 ? buffer_sizes[buffer_index]: size, 0, mapped_memory);
	HANDEL(lastResult)
}

void unmapMemory() {
	vkUnmapMemory(device, device_memory);
}

void destroyMemory() {
	for(VkBuffer buffer : buffers) {
		vkDestroyBuffer(device, buffer, nullptr);
	}
	vkFreeMemory(device, device_memory, nullptr);
}