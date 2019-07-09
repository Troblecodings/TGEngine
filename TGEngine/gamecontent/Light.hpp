#pragma once

#include "../Stdbase.hpp"
#include "../pipeline/buffer/UniformBuffer.hpp"
#include "Actor.hpp"

namespace tge {
	namespace gmc {

		extern UniformBuffer lightbuffer; // the light buffer for the postion of the light

		/*
		 * Internal method to initialize the light buffer
		 */
		INTERNAL
			SINCE(0, 0, 4)
			void initLight();

		class LightActor : public Actor {

		};
	}
}