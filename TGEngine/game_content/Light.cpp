#include "Light.hpp"

UniformBuffer light_buffer;

void initLight() {
	light_buffer = {
			sizeof(glm::vec3),
	        { VK_SHADER_STAGE_VERTEX_BIT }
	};
	createUniformBuffer(&light_buffer);
}

void setLightPosition(glm::vec3 pos) {
	fillUniformBuffer(&light_buffer, &pos, sizeof(glm::vec3));
}