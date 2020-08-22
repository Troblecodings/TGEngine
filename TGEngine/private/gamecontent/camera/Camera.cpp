#include "../../../public/gamecontent/camera/Camera.hpp"
#include "../../../public/io/Mouse.hpp"
#include "../../../public/io/Keyboard.hpp"
#include "../../../public/pipeline/buffer/UniformBuffer.hpp"

namespace tge {
	namespace gmc {

		using namespace tge::buf;

		std::vector<Camera*> cameras_on_scene;
		float multiplier = 1;
		size_t active_camera = 0;

		void initCameras() {
			if (cameras_on_scene.size() > 0) tge::io::addListener(__impl_input_handle);
			if (cameras_on_scene.size() > 0) tge::io::keyboardHandler.push_back(__impl_keyinput_handle);
		}

		void createCamera(Camera* camera) {
			TG_VECTOR_APPEND_NORMAL(cameras_on_scene, camera)
				active_camera = camera->camera_index = lastSize;
		}

		void updateCamera(int width, int height) {
			Camera* ptr = cameras_on_scene[active_camera];
			glm::mat4 matrix = glm::perspective(ptr->fov, width / (float)height, ptr->nearClipPlain, ptr->farClipPlain) // Perspective
				* (glm::mat4_cast(glm::quat(ptr->rotation)) * glm::translate(glm::mat4(1), ptr->translation)) // Camera Matrix -> RT
				* (glm::translate(glm::mat4(1), ptr->worldTranslation) * glm::mat4_cast(glm::quat(ptr->worldRotation)) * glm::scale(glm::mat4(1), ptr->worldScale)); // World rotation -> TRS
			fillUniformBuffer(TRANSFORM_BUFFER, &matrix, sizeof(glm::mat4));
		}

		void __impl_input_handle(glm::vec2 pos, glm::vec2 delta) {
			Camera* cam = cameras_on_scene[active_camera];
			if (cam->mouse_input_handler) {
				cam->mouse_input_handler(cam, pos, delta);
			}
		}

		void __impl_keyinput_handle(uint16_t chr, bool down) {
			Camera* cam = cameras_on_scene[active_camera];
			if (cam->key_input_handler) {
				cam->key_input_handler(cam, chr, down);
			}
		}
	}
}
