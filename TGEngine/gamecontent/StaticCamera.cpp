#include "StaticCamera.hpp"

namespace tge {
	namespace gmc {

		void createStaticCamera(Camera* camera) {
			camera->camera = glm::lookAt(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			camera->mouse_input_handler = __impl_mouseinputHandleSC;
			camera->key_input_handler = __impl_keyinput_handleSC;
			createCamera(camera);
		}

		void __impl_mouseinputHandleSC(Camera* camera, glm::vec2 pos, glm::vec2 delta) {
			if(!tg_io::FIRST_MOUSE_BUTTON)return;
			camera->applyWorldRotation(0, 1, 0, delta.x * -camera->speed);
			camera->applyWorldRotation(1, 0, 0, delta.y * -camera->speed);
			updateCamera(window_list[0]->width, window_list[0]->height);
		}

		void __impl_keyinput_handleSC(Camera* camera, uint16_t chr, bool down) {
			if (chr == 'W' && down) {
				camera->applyWorldScale(1.2, 1.2, 1.2);
				updateCamera(window_list[0]->width, window_list[0]->height);
			}
			else if (chr == 'S' && down) {
				camera->applyWorldScale(0.8, 0.8, 0.8);
				updateCamera(window_list[0]->width, window_list[0]->height);
			}
		}
	}
}