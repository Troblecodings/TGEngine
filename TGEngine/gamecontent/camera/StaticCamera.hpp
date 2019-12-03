#pragma once

#include "../../Stdbase.hpp"
#include "Camera.hpp"

namespace tge {
	namespace gmc {

		/*
		 * This creates a camera that is static
		 * but the rendered objects can be turned
		 *
		 * <strong>This API is deprecated!</strong>
		 */
		void createStaticCamera(Camera* camera);

		/* This is an internal function which is executed on
		 * Mouse input as defined by the Mouse.hpp
		 *
		 * <strong>This API is deprecated!</strong>
		 */
		void __impl_mouseinputHandleSC(Camera* camera, glm::vec2 pos, glm::vec2 delta);

		/* This is an internal function which is executed on
		 * Keyboard input as defined by the Keyboard.hpp
		 *
		 * <strong>This API is deprecated!</strong>
		 */
		void __impl_keyinput_handleSC(Camera* camera, uint16_t chr, bool down);
	}
}