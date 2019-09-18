#pragma once

#include "../../Stdbase.hpp"
#include "Camera.hpp"

namespace tge {
	namespace gmc {

		void createFirstPersonCamera(Camera* camera);

		void __impl_mouseinputhandlerFPC(Camera* camera, glm::vec2 pos, glm::vec2 delta);

		void __impl_keyboard_handleFPC(Camera* camera, uint16_t chr, bool down);
	}
}