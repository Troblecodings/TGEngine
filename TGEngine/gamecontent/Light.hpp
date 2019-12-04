#pragma once

#include "../Stdbase.hpp"
#include "../pipeline/buffer/UniformBuffer.hpp"
#include "Actor.hpp"

namespace tge {
	namespace gmc {

		/*
		 * This is the maximum count of lights used in the scene
		 */
		extern uint32_t lightCount;

		/*
		 * This holds the lights information
		 *
		 * <ul>
		 * <li><strong class='atr'>pos</strong> is the position in the space</li>
		 * <li><strong class='atr'>intensity</strong> is the intensity of the light</li>
		 * <li><strong class='atr'>id</strong> is the id in the light array</li></ul>
		 * <strong>This API is deprecated!</strong>
		 */
		class LightActor {

		private:
			glm::vec4 pos;
			glm::vec4 intensity;
			uint32_t  id;

		public:
			LightActor(uint32_t intensity, glm::vec3 color, glm::vec3 pos);

			void updateLight();
		};

		/* This holds references to the light actors
		 *
         * <strong>This API is deprecated!</strong>
		 */
		extern std::vector<LightActor*> lights;
	}
}