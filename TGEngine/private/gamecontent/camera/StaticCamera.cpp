#include "../../../public/gamecontent/camera/StaticCamera.hpp"
#include "../../../public/pipeline/window/Window.hpp"

namespace tge {
	namespace gmc {

		void createStaticCamera(Camera* camera) {
			camera->mouse_input_handler = __impl_mouseinputHandleSC;
			camera->key_input_handler = __impl_keyinput_handleSC;
			createCamera(camera);
		}

		void __impl_mouseinputHandleSC(Camera* camera, glm::vec2 pos, glm::vec2 delta) {
			if (!tge::io::FIRST_MOUSE_BUTTON)return;
			camera->worldRotation += glm::vec3(delta.y, delta.x, 0) * camera->speed;
			updateCamera(win::mainWindowWidth, win::mainWindowHeight);
		}

		void __impl_keyinput_handleSC(Camera* camera, uint16_t chr, bool down) {
			if (chr == 'W' && down) {
				camera->worldScale += glm::vec3(0.2, 0.2, 0.2);
				updateCamera(win::mainWindowWidth, win::mainWindowHeight);
			} else if (chr == 'S' && down) {
				camera->worldScale -= glm::vec3(0.2, 0.2, 0.2);
				updateCamera(win::mainWindowWidth, win::mainWindowHeight);
			}
		}
	}
}