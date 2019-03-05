#include "StaticCamera.hpp"

void createStaticCamera(Camera * camera)
{
	camera->camera = glm::lookAt(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	camera->mouse_input_handler = __impl_mouseinputHandleSC;
	createCamera(camera);
}

void __impl_mouseinputHandleSC(Camera * camera, glm::vec2 pos, glm::vec2 delta)
{
	if (!tg_io::FIRST_MOUSE_BUTTON)return;
	camera->applyWorldRotation(0, 1, 0, delta.x * -camera->speed);
	camera->applyWorldRotation(1, 0, 0, delta.y * -camera->speed);
	updateCamera(window_list[0]->width, window_list[0]->height);
}
