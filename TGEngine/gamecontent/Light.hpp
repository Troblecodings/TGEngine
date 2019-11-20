#pragma once

#include "../Stdbase.hpp"
#include "../pipeline/buffer/UniformBuffer.hpp"
#include "Actor.hpp"

namespace tge {
	namespace gmc {

		extern uint32_t lightCount;

		class LightActor {

		private:
			glm::vec4 pos;
			glm::vec4 intensity;
			uint32_t id;

		public:
			LightActor(uint32_t intensity, glm::vec3 color, glm::vec3 pos);

			void updateLight();
		};

		extern std::vector<LightActor*> lights;
	}
}