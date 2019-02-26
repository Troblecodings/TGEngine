#pragma once
#include "../Stdbase.hpp"
#include "../pipeline/buffer/VertexBuffer.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../io/Mouse.hpp"
#include "../io/Keyboard.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

struct Camera {
	OPT INPUT
	double speed = 0.01f; // Speed of the camera movement - Usually less then 1 and greater then 0
	float fov = (float)PI * 0.5f; // Field of view in radians
	float near_clip_plain = 0.01f; // The nearest thing to the camera
	float far_clip_plain = 1000.0f; // The farest thing from the camera
	void (*mouse_input_handler)(Camera* camera, glm::vec2, glm::vec2) = NULL; // A handle for the camera (How should it handle mouse movement?)
	void(*key_input_handler)(Camera* camera, uint16_t chr, bool down) = NULL; // A handle for the camera (How should it handle key input?)

	OUTPUT 
	glm::mat4 world_transform = glm::mat4(1.0f); // The world transform for all verticies in the world 
	glm::mat4 camera = glm::mat4(1.0f); // The camera transforms done with glm::lookAt
	glm::mat4 matrix; // The actual computed matrix -> Calculated: perspective * camera * world_transform
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.5f); // The position of the camera in world space
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f); // The direction into which the camera faces
	size_t camera_index;

	/*  
	 * Applies rotation to the world_transform matrix
	 *  -> used for static cameras - e.g. Editor
	 */
	SINCE(0, 0, 3)
	void applyWorldRotation(double x, double y, double z, double angle);

	/*
     * Applies translation to the world_transform matrix
     *  -> used for static cameras - e.g. Editor
     */
	SINCE(0, 0, 4)
	void applyWorldTranslation(double x, double y, double z);

	/*
     * Applies scale to the world_transform matrix
     *  -> used for static cameras - e.g. Editor
     */
	SINCE(0, 0, 4)
	void applyWorldScale(double x, double y, double z);

	/*
     * Applies rotation to the camera matrix
     *  -> used for first person cameras - e.g. FPS
     */
	SINCE(0, 0, 4)
	void applyCameraRotation(double x, double y, double z, double angle);

	/*
     * Applies translation to the camera matrix
     *  -> used for first person cameras - e.g. FPS
     */
	SINCE(0, 0, 4)
	void applyCameraTranslation(double x, double y, double z);

};

extern UniformBuffer camera_uniform; // The uniform buffer for the camera matrix
extern UniformBuffer ui_camera_uniform; // The uniform buffer for the camera matrix in the UI
extern std::vector<Camera*> cameras_on_scene; // Contains all cameras on the scene
extern size_t active_camera; // The index of the active camera
extern float multiplier; // defines the y mutliplier for the ui
extern float multiplierx; // defines the x mutliplier for the ui

/*
 * Updates the active camera camera matrix and uploads it to the uniform buffer in the shader
 *   -> Also sets perspective with the given width and height
 */
SINCE(0, 0, 2)
void updateCamera(int width, int height);

/*
 * Internal method. You shouldn't use it ... I mean you can but why would you?
 */
INTERNAL
SINCE(0, 0, 4)
void __impl_input_handle(glm::vec2 pos, glm::vec2 delta);

/*
 * Internal method. You shouldn't use it ... I mean you can but why would you?
 */
INTERNAL
SINCE(0, 0, 4)
void __impl_keyinput_handle(uint16_t chr, bool down);

/*
 * Initalizes the camera Uniformbuffer and etc.
 */
SINCE(0, 0, 4)
void initCameras();

/* 
 * Creates a camera. This method is not "Internal" but it is still not the way it is meant to be done. Consider using createStaticCamera 
 * from StaticCamera.hpp for example.
 *
 *   -> mouse_input_handler will not be set
 *   -> key_input_handler will not be set
 */
SINCE(0, 0, 2)
void createCamera(Camera* camera);