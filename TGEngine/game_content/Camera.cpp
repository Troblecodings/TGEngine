#include "Camera.hpp"

void createCamera(Camera* camera) {
	camera->uniform = {
		sizeof(glm::mat4),
	   { VK_SHADER_STAGE_VERTEX_BIT }
	};
	camera->world_transform = glm::mat4(1.0f);
	createUniformBuffer(&camera->uniform);
	addListener(camera);
}

void Camera::applyWorldRotation(double x, double y, double z, double angle) {
	this->world_transform = glm::rotate(this->world_transform, (float)angle, glm::vec3(x, y, z));
}

void Camera::updateCamera(float width, float height) {
	glm::mat4 camera = glm::lookAt(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), width / height, 0.01f, 1000.0f);
	projection[1][1] *= -1;
	this->matrix = projection * camera * this->world_transform;
	fillUniformBuffer(&this->uniform, (uint8_t*)&this->matrix, sizeof(glm::mat4));
}

void Camera::mouse_move(glm::vec2 pos, glm::vec2 delta)
{
	if (!FIRST_MOUSE_BUTTON)return;
	this->applyWorldRotation(0, 1, 0, delta.x * -this->speed * PIx2);
	this->applyWorldRotation(1, 0, 0, delta.y * -this->speed * PIx2);
	this->updateCamera(window_list[0]->width, window_list[0]->height);
}
