#include "Camera.hpp"
#include "../io/Mouse.hpp"
#include "../io/Keyboard.hpp"

namespace tge {
	namespace gmc {

		std::vector<Camera*> cameras_on_scene;
		UniformBuffer cameraUBO;
		float multiplier = 1;
		size_t active_camera = 0;

		void initCameras() {
			cameraUBO = UniformBuffer(sizeof(glm::mat4), VK_SHADER_STAGE_VERTEX_BIT, 0);

			if (cameras_on_scene.size() > 0) tg_io::addListener(__impl_input_handle);
			if (cameras_on_scene.size() > 0) tg_io::addKeyListener(__impl_keyinput_handle);
		}

		void createCamera(Camera* camera) {
			TG_VECTOR_APPEND_NORMAL(cameras_on_scene, camera)
			active_camera = camera->camera_index = lastSize;
		}

		void updateCamera(int width, int height) {
			Camera* ptr = cameras_on_scene[active_camera];
			glm::quat qut = glm::quat(ptr->cameraRotation);
			glm::mat4 projection = glm::perspective(ptr->fov, width / (float)height, ptr->near_clip_plain, ptr->far_clip_plain);
			ptr->matrix = projection * glm::translate(glm::mat4(1), ptr->translation) 
				* (glm::translate(glm::mat4(1), ptr->worldTranslation) * glm::mat4_cast(glm::quat(ptr->worldRotation)) * glm::scale(glm::mat4(1), ptr->worldScale));
			fillUniformBuffer(&cameraUBO, &ptr->matrix, sizeof(glm::mat4));
		}

		void __impl_input_handle(glm::vec2 pos, glm::vec2 delta) {
			Camera* cam = cameras_on_scene[active_camera];
			if(cam->mouse_input_handler) {
				cam->mouse_input_handler(cam, pos, delta);
			}
		}

		void __impl_keyinput_handle(uint16_t chr, bool down) {
			Camera* cam = cameras_on_scene[active_camera];
			if(cam->key_input_handler) {
				cam->key_input_handler(cam, chr, down);
			}
		}
	}
}
