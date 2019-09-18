#include "FirstPersonCamera.hpp"
#include "../../pipeline/window/Window.hpp"

namespace tge {
	namespace gmc {

		void createFirstPersonCamera(Camera* camera) {
			camera->mouse_input_handler = __impl_mouseinputhandlerFPC;
			camera->key_input_handler = __impl_keyboard_handleFPC;
			createCamera(camera);
		}

		void __impl_mouseinputhandlerFPC(Camera* camera, glm::vec2 pos, glm::vec2 delta) {
			camera->rotation += glm::vec3(delta.y, delta.x, 0) * camera->speed;
			updateCamera(windowList[0]->width, windowList[0]->height);
		}

		void __impl_keyboard_handleFPC(Camera* camera, uint16_t chr, bool down) {
			if (!down) {
				glm::quat mp = glm::quat(glm::vec3(camera->rotation.x, 0, 0));
				if (chr == 'W') camera->translation += glm::vec3(0, 0, 0.1) * mp;
				if (chr == 'S')	camera->translation += glm::vec3(0, 0, -0.1) * mp;
				if (chr == 'A')	camera->translation += glm::vec3(0.1, 0, 0) * mp;
				if (chr == 'D') camera->translation += glm::vec3(-0.1, 0, 0) * mp;
				updateCamera(windowList[0]->width, windowList[0]->height);
			}
		}
	}
}