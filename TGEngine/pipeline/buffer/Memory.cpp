#include "Memory.hpp"

std::vector<VkBuffer*> buffers = {};
std::vector<VkDeviceSize> buffer_sizes = {};
std::vector<VkDeviceSize> buffer_offsets = { 0 };
VkDeviceMemory device_memory;
VkDeviceSize _impl_size;
VkMemoryRequirements last_requirements;

uint32_t addBuffer(VkBuffer* buffer) {
	vkGetBufferMemoryRequirements(device, *buffer, &last_requirements);

	_impl_size += last_requirements.size;

	uint32_t csize = buffer_offsets.size();
	buffer_offsets.resize(csize + 1);
	buffer_offsets[csize] = buffer_offsets[csize - 1] + last_requirements.size;

	csize = buffer_sizes.size();
	buffer_sizes.resize(csize + 1);
	buffer_sizes[csize] = last_requirements.size;

	buffers.resize(csize + 1);
	buffers[csize] = buffer;
	return csize;
}
 
void allocateAllBuffers() {
	vlib_buffer_memory_allocate_info.allocationSize = _impl_size;
	vlib_buffer_memory_allocate_info.memoryTypeIndex = vlib_device_host_visible_coherent_index;
	last_result = vkAllocateMemory(device, &vlib_buffer_memory_allocate_info, nullptr, &device_memory);
	HANDEL(last_result)
	
	for (int i = 0; i < buffers.size();i++) {
		last_result = vkBindBufferMemory(device, *buffers[i], device_memory, buffer_offsets[i]);
		HANDEL(last_result)
	}
}

void mapMemory(uint32_t buffer_index, void** mapped_memory) {
	last_result = vkMapMemory(device, device_memory, buffer_offsets[buffer_index], buffer_sizes[buffer_index], 0, mapped_memory);
	HANDEL(last_result)
}

void unmapMemory() {
	vkUnmapMemory(device, device_memory);
}

void destroyMemory() {
	for (VkBuffer buffer : buffers) {
		vkDestroyBuffer(device, buffer, nullptr);
	}
	vkFreeMemory(device, device_memory, nullptr);
}