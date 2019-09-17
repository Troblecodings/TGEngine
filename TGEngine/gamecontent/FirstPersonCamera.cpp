#include "FirstPersonCamera.hpp"
#include "..\pipeline\window\Window.hpp"

namespace tge {
	namespace gmc {

		void createFirstPersonCamera(Camera* camera) {
			camera->mouse_input_handler = __impl_mouseinputhandlerFPC;
			camera->key_input_handler = __impl_keyboard_handleFPC;
			createCamera(camera);
		}

		void __impl_mouseinputhandlerFPC(Camera* camera, glm::vec2 pos, glm::vec2 delta) {
			camera->cameraRotation += glm::vec3(delta * camera->speed, 0);
			updateCamera(windowList[0]->width, windowList[0]->height);
		}

		void __impl_keyboard_handleFPC(Camera* camera, uint16_t chr, bool down) {
			if (!down) {
				if (chr == 'W') camera->translation += glm::vec3(0, 0.1, 0) * camera->rotation;
				if (chr == 'S')	camera->translation += glm::vec3(0, -0.1, 0) * camera->rotation;
				if (chr == 'A')	camera->translation += glm::vec3(0.1, 0, 0) * camera->rotation;
				if (chr == 'D') camera->translation += glm::vec3(-0.1, 0, 0) * camera->rotation;
				updateCamera(windowList[0]->width, windowList[0]->height);
			}
		}
	}
}