#include "FirstPersonCamera.hpp"

void createFirstPersonCamera(Camera * camera)
{
	camera->mouse_input_handler = __impl_mouseinputhandlerFPC;
	camera->key_input_handler = __impl_keyboard_handleFPC;
	createCamera(camera);
}

void __impl_mouseinputhandlerFPC(Camera* camera, glm::vec2 pos, glm::vec2 delta)
{
	camera->applyCameraRotation(glm::vec3(delta.y, delta.x, 0));
	updateCamera(window_list[0]->width, window_list[0]->height);
}

void __impl_keyboard_handleFPC(Camera * camera, uint16_t chr, bool down) {
	if (!down) {
		switch (chr)
		{
		case 'W':
			//camera->applyCameraTranslation(0.1 * camera->direction.x, 0.1 * camera->direction.y, 0);
			break;
		case 'S':
			//camera->applyCameraTranslation(-0.1 * camera->direction.x, -0.1 * camera->direction.y, 0);
			break;
		case 'A':
			//camera->applyCameraTranslation(-0.1 * camera->direction.y, 0.1 * camera->direction.x, 0);
			break;
		case 'D':
			//camera->applyCameraTranslation(0.1 * camera->direction.y, -0.1 * camera->direction.x, 0);
			break;
		default:
			break;
		}
		updateCamera(window_list[0]->width, window_list[0]->height);
	}
}