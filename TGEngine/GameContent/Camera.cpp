#include "Camera.hpp"

void Rotation::setRotations(float x, float y, float z) {
	this->x_rotation = PIx2 * x;
	this->y_rotation = PIx2 * y;
	this->z_rotation = PIx2 * z;
}

void createCamera(Camera* camera) {
	camera->buffer = {
		sizeof(Rotation),
	    { 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT }
	};
	createUniformBuffer(&camera->buffer);
}

void Camera::updateCamera() {
	fillUniformBuffer(&this->buffer, (uint8_t*)&this->rotation, sizeof(this->rotation));
}