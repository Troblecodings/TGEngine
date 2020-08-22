#pragma once

#include "../../../public/Stdbase.hpp"
#include "../../../public/gamecontent/camera/Camera.hpp"

namespace tge {
	namespace gmc {

		/*
		 * Creates a first person camera with the according perspective
		 * and movement input handling
		 * 
		 * <strong>This API is deprecated!</strong>
		 */
		void createFirstPersonCamera(Camera* camera);

		/* Internal function which is executed on
		 * mouse input as defined by Mouse.hpp
		 *
		 * <strong>This API is deprecated!</strong>
		 */
		void __impl_mouseinputhandlerFPC(Camera* camera, glm::vec2 pos, glm::vec2 delta);

		/* Internal function which is executed on
                 * keyboard input as defined by Keyboard.hpp
                 * <strong>This API is deprecated!</strong>
                 */
		void __impl_keyboard_handleFPC(Camera* camera, uint16_t chr, bool down);
	}
}
