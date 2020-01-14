#pragma once

#include "../../Stdbase.hpp"
#include "Camera.hpp"

namespace tge {
	namespace gmc {

		/*
		 * Creates a first person camera with the according perspective
		 * and movement input handling
		 * 
		 * <strong>This API is deprecated!</strong>
		 */
		void createFirstPersonCamera(Camera* camera);

		/* This is an internal function which is executed on
		 * Mouse input as defined by the Mouse.hpp
		 *
		 * <strong>This API is deprecated!</strong>
		 */
		void __impl_mouseinputhandlerFPC(Camera* camera, glm::vec2 pos, glm::vec2 delta);

		/* This is an internal function which is executed on
         * Keyboard input as defined by the Keyboard.hpp
         *
         * <strong>This API is deprecated!</strong>
         */
		void __impl_keyboard_handleFPC(Camera* camera, uint16_t chr, bool down);
	}
}
