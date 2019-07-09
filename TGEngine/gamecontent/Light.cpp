#include "Light.hpp"

namespace tge {
	namespace gmc {

		UniformBuffer lightbuffer;

		void initLight() {
			lightbuffer = UniformBuffer(sizeof(glm::vec3), VK_SHADER_STAGE_FRAGMENT_BIT);
			lightbuffer.descriptor.binding = 1;
		}
	}
}