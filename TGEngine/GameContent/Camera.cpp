#include "Camera.hpp"

void createCamera(Camera* camera) {
	camera->uniform = {
		sizeof(glm::mat4),
	   { VK_SHADER_STAGE_VERTEX_BIT }
	};
	camera->rotation = glm::mat4(1);
	createUniformBuffer(&camera->uniform);
	addListener(camera);
}

void Camera::applyRotation(float x, float y, float z, float angle) {
	this->rotation = glm::rotate(this->rotation, angle, glm::vec3(x, y, z));
}

void Camera::updateCamera() {
	fillUniformBuffer(&this->uniform, (uint8_t*)&this->rotation, sizeof(glm::mat4));
}

void Camera::mouse_move(glm::vec2 pos, glm::vec2 delta)
{
	this->applyRotation(0, 1, 0, delta.x * -this->speed * PIx2);
	this->applyRotation(1, 0, 0, delta.y * this->speed * PIx2);
	this->updateCamera();
}
