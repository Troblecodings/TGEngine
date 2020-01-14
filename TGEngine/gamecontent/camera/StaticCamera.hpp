#pragma once

#include "../../Stdbase.hpp"
#include "Camera.hpp"

namespace tge {
	namespace gmc {

		/* This creates a static camera
		 * but the rendered objects can be turned
		 *
		 * <strong>This API is deprecated!</strong>
		 */
		void createStaticCamera(Camera* camera);

		/* Internal function which is executed on
		 * mouse input as defined by Mouse.hpp
		 *
		 * <strong>This API is deprecated!</strong>
		 */
		void __impl_mouseinputHandleSC(Camera* camera, glm::vec2 pos, glm::vec2 delta);

		/* Internal function which is executed on
		 * keyboard input as defined by Keyboard.hpp
		 *
		 * <strong>This API is deprecated!</strong>
		 */
		void __impl_keyinput_handleSC(Camera* camera, uint16_t chr, bool down);
	}
}
