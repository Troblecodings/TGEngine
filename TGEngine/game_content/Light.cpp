#include "Light.hpp"

UniformBuffer light_buffer;

void initLight() {
	light_buffer = UniformBuffer(sizeof(glm::vec3), VK_SHADER_STAGE_FRAGMENT_BIT);
	light_buffer.descriptor.binding = 1;
}

void setLightPosition(glm::vec3 pos) {
	fillUniformBuffer(&light_buffer, &pos, sizeof(glm::vec3));
}