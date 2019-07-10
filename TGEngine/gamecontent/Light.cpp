#include "Light.hpp"

namespace tge {
	namespace gmc {

		std::vector<LightActor*> lights;
		UniformBuffer lightbuffer;
		uint32_t lightCount = 0;

		void initLight() {
			lightbuffer = UniformBuffer(sizeof(glm::vec3) * 2 * 100, VK_SHADER_STAGE_FRAGMENT_BIT);
			lightbuffer.descriptor.binding = 1;
		}

		LightActor::LightActor(glm::vec3 intensity, glm::vec3 pos) : intensity(intensity), pos(pos){
			this->id = lightCount++;
		}

		void LightActor::updateLight() {
			fillUniformBuffer(&lightbuffer, this, 2 * sizeof(glm::vec3), (this->id * 2 * sizeof(glm::vec3)) + ((this->id + this->id + 1) * sizeof(uint32_t)) /* I have no clue why I need to add this
																															  * This is just a work around for the light color values
																															  */);
		}
	}
}