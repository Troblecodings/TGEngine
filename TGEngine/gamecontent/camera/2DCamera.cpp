#include "2DCamera.hpp"

void tge::gmc::setTopDownCamera(TopDownCamera* camera)
{
	glm::mat4 mat = { camera->screenheight / (float)camera->screenwidth, 0, 0, 0,
					  0, 1, 0, 0,
					  0, 0, 0, 0,
					  camera->positionx, camera->positiony, 0, 1 };
	fillUniformBuffer(&cameraUBO, &mat, sizeof(glm::mat4));
}
