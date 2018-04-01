#include "Camera.hpp"

void Rotation::setRotations(float x, float y, float z) {
	this->x_rotation = PIx2 * x;
	this->y_rotation = PIx2 * y;
	this->z_rotation = PIx2 * z;
}

void createCamera(Camera* camera) {
}

void Camera::updateCamera() {
}