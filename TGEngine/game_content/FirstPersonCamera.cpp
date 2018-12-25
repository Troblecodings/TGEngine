#include "FirstPersonCamera.hpp"

void createFirstPersonCamera(Camera * camera)
{
	camera->camera = glm::lookAt(camera->position, camera->position + camera->direction, glm::vec3(0.0f, 0.0f, 1.0f));
	camera->mouse_input_handler = __impl_mouseinputhandlerFPC;
	createCamera(camera);
}

void __impl_mouseinputhandlerFPC(glm::vec2 pos, glm::vec2 delta, Camera * camera)
{
	camera->applyCameraRotation(0, 0, 1, delta.x * -(float)camera->speed * (float)PIx2);
	camera->applyCameraRotation(1, 0, 0, delta.y * (float)camera->speed * (float)PIx2);
	updateCamera(window_list[0]->width, window_list[0]->height);
}
