#include "FirstPersonCamera.hpp"

void createFirstPersonCamera(Camera * camera)
{
	camera->camera = glm::lookAt(camera->position, camera->position + camera->direction, glm::vec3(0.0f, 0.0f, 1.0f));
	camera->mouse_input_handler = __impl_mouseinputhandlerFPC;
	camera->key_input_handler = __impl_keyboard_handleFPC;
	createCamera(camera);
}

void __impl_mouseinputhandlerFPC(Camera * camera, glm::vec2 pos, glm::vec2 delta)
{
	camera->applyCameraRotation(0, 0, 1, delta.x * -(float)camera->speed);
	camera->applyCameraRotation(1, 0, 0, delta.y * (float)camera->speed);
	updateCamera(window_list[0]->width, window_list[0]->height);
}

void __impl_keyboard_handleFPC(Camera * camera, uint16_t chr, bool down) {
	if (!down) {
		switch (chr)
		{
		case 'W':
			camera->applyCameraTranslation(0.1, 0.1, 0);
			break;
		case 'S':
			camera->applyCameraTranslation(-0.1, -0.1, 0);
			break;
		case 'A':
			camera->applyCameraTranslation(0.1, 0.1, 0);
			break;
		case 'D':
			camera->applyCameraTranslation(0.1, 0.1, 0);
			break;
		default:
			break;
		}
		updateCamera(window_list[0]->width, window_list[0]->height);
	}
}