#pragma once
#include "../../Stdbase.hpp"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../Actor.hpp"

namespace tge {
	namespace gmc {

		struct Camera {
			float speed = 0.01f; // Speed of the camera movement - Usually less then 1 and greater then 0
			
			float fov = (float)PI * 0.5f; // Field of view in radians
			float nearClipPlain = 0.01f; // The nearest thing to the camera
			float farClipPlain = 1000.0f; // The farest thing from the camera
			
			void (*mouse_input_handler)(Camera* camera, glm::vec2, glm::vec2) = NULL; // A handle for the camera (How should it handle mouse movement?)
			void(*key_input_handler)(Camera* camera, uint16_t chr, bool down) = NULL; // A handle for the camera (How should it handle key input?)

			glm::vec3 rotation{};
			glm::vec3 translation{};

			glm::vec3 worldTranslation{};
			glm::vec3 worldRotation{};
			glm::vec3 worldScale{ 1, 1, 1};

			size_t camera_index;
		};

		extern std::vector<Camera*> cameras_on_scene; // Contains all cameras on the scene
		extern size_t active_camera; // The index of the active camera
		extern float multiplier; // defines the y mutliplier for the ui

		/*
		 * Updates the active camera camera matrix and uploads it to the uniform buffer in the shader
		 *   -> Also sets perspective with the given width and height
		 */
		void updateCamera(int width, int height);

		/*
		 * Internal method. You shouldn't use it ... I mean you can but why would you?
		 */
		void __impl_input_handle(glm::vec2 pos, glm::vec2 delta);

		/*
		 * Internal method. You shouldn't use it ... I mean you can but why would you?
		 */
		void __impl_keyinput_handle(uint16_t chr, bool down);

		/*
		 * Initalizes the camera Uniformbuffer and etc.
		 */
		void initCameras();

		/*
		 * Creates a camera. This method is not "Internal" but it is still not the way it is meant to be done. Consider using createStaticCamera
		 * from StaticCamera.hpp for example.
		 *
		 *   -> mouse_input_handler will not be set
		 *   -> key_input_handler will not be set
		 */
		void createCamera(Camera* camera);

	}
}