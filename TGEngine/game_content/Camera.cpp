#include "Camera.hpp"

std::vector<Camera*> cameras_on_scene;
UniformBuffer camera_uniform;
size_t active_camera = 0;

void initCameras() {
	camera_uniform = {
	sizeof(glm::mat4),
	{ VK_SHADER_STAGE_VERTEX_BIT }
	};
	createUniformBuffer(&camera_uniform);
	addListener(__impl_input_handle);
}

void createCamera(Camera* camera) {
	TG_VECTOR_APPEND_NORMAL(cameras_on_scene, camera)
	camera->camera_index = last_size;
}

void Camera::applyWorldRotation(double x, double y, double z, double angle) 
{
	this->world_transform = glm::rotate(this->world_transform, (float)angle, glm::vec3(x, y, z));
}

void Camera::applyWorldTranslation(double x, double y, double z)
{
	this->world_transform = glm::translate(this->world_transform, glm::vec3(x, y, z));
}

void Camera::applyWorldScale(double x, double y, double z)
{
	this->world_transform = glm::scale(this->world_transform, glm::vec3(x, y, z));
}

void Camera::applyCameraRotation(double x, double y, double z, double angle)
{
	this->direction = glm::rotate(this->direction, (float)angle, glm::vec3(x, y, z));
}

void Camera::applyCameraTranslation(double x, double y, double z)
{
	this->position += glm::vec3(x, y, z);

}

void updateCamera(int width, int height) {
	Camera* ptr = cameras_on_scene[active_camera];
	ptr->camera = glm::lookAt(ptr->position, ptr->position + ptr->direction, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 projection = glm::perspective(ptr->fov, width / (float)height, ptr->near_clip_plain, ptr->far_clip_plain);
	projection[1][1] *= -1;
	ptr->matrix = projection * ptr->camera * ptr->world_transform;
	fillUniformBuffer(&camera_uniform, (uint8_t*)&ptr->matrix, sizeof(glm::mat4));
}

void __impl_input_handle(glm::vec2 pos, glm::vec2 delta)
{
	if (cameras_on_scene[active_camera]->mouse_input_handler)cameras_on_scene[active_camera]->mouse_input_handler(pos, delta, cameras_on_scene[active_camera]);
}