#pragma once
#include "..\Stdbase.hpp"
#include "..\Pipeline\Buffer\VertexBuffer.hpp"
#include <glm\gtc\matrix_transform.hpp>
#include "..\IO\Mouse.hpp"

struct Camera : Mouse{
	INPUT float speed;

	OUTPUT UniformBuffer uniform;
	OUTPUT glm::mat4 rotation;

	SINCE(0, 0, 3)
	void applyRotation(float x, float y, float z, float angle);

	SINCE(0, 0, 2)
	void updateCamera();

	void mouse_move(glm::vec2 pos, glm::vec2 delta) override;
};

SINCE(0, 0, 2)
void createCamera(Camera* camera);