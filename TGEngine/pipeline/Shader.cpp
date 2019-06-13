#include "Shader.hpp"

std::vector<VkVertexInputAttributeDescription> description_attributes;

void createShaderInput(uint32_t location, uint32_t offset, VkFormat format) {
	TG_VECTOR_GET_SIZE_AND_RESIZE(description_attributes)
	description_attributes[last_size].location = location;
	description_attributes[last_size].offset = offset;
	description_attributes[last_size].format = format;
	description_attributes[last_size].binding = 0;
}

void destroyShaders() {
}