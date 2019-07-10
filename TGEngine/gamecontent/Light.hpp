#pragma once

#include "../Stdbase.hpp"
#include "../pipeline/buffer/UniformBuffer.hpp"
#include "Actor.hpp"

namespace tge {
	namespace gmc {

		extern UniformBuffer lightbuffer; // the light buffer for the postion and intensity of the light(s)
		extern uint32_t lightCount;

		/*
		 * Internal method to initialize the light buffer
		 */
		INTERNAL
			SINCE(0, 0, 4)
			void initLight();

		class LightActor {

		private:
			glm::vec3 pos;
			glm::vec3 intensity;
			uint32_t id;

		public:
			LightActor(glm::vec3 intensity, glm::vec3 pos);

			void updateLight();
		};

		extern std::vector<LightActor*> lights;
	}
}