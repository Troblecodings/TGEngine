#include "StaticCamera.hpp"
#include "../../pipeline/window/Window.hpp"

namespace tge {
	namespace gmc {

		void createStaticCamera(Camera* camera) {
			camera->mouse_input_handler = __impl_mouseinputHandleSC;
			camera->key_input_handler = __impl_keyinput_handleSC;
			createCamera(camera);
		}

		void __impl_mouseinputHandleSC(Camera* camera, glm::vec2 pos, glm::vec2 delta) {
			if(!tg_io::FIRST_MOUSE_BUTTON)return;
			camera->worldRotation += glm::vec3(delta.y, delta.x , 0) * camera->speed;
			updateCamera(windowList[0]->width, windowList[0]->height);
		}

		void __impl_keyinput_handleSC(Camera* camera, uint16_t chr, bool down) {
			if (chr == 'W' && down) {
				camera->worldScale += glm::vec3(0.2, 0.2, 0.2);
				updateCamera(windowList[0]->width, windowList[0]->height);
			}
			else if (chr == 'S' && down) {
				camera->worldScale -= glm::vec3(0.2, 0.2, 0.2);
				updateCamera(windowList[0]->width, windowList[0]->height);
			}
		}
	}
}