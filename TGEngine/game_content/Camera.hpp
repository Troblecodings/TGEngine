#pragma once
#include "..\Stdbase.hpp"
#include "..\Pipeline\Buffer\VertexBuffer.hpp"
#include <glm\gtc\matrix_transform.hpp>
#include "..\IO\Mouse.hpp"

struct Camera : Mouse{
	INPUT OPT double speed = 0.001;

	OUTPUT 
	UniformBuffer uniform;
	glm::mat4 world_transform;
	glm::mat4 matrix;

	SINCE(0, 0, 3)
	void applyWorldRotation(double x, double y, double z, double angle);

	SINCE(0, 0, 2)
	void updateCamera(float width, float height);

	void mouse_move(glm::vec2 pos, glm::vec2 delta) override;
};

SINCE(0, 0, 2)
void createCamera(Camera* camera);