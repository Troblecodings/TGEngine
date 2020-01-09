#include "2DCamera.hpp"
#include "../../pipeline/buffer/UniformBuffer.hpp"

using namespace tge::buf;

void tge::gmc::setTopDownCamera(TopDownCamera* camera) {
	glm::mat4 mat =
	{ camera->screenheight / (float)camera->screenwidth, 0, 0, camera->positionx,
		0, 1, 0, camera->positiony,
		0, 0, -0.1, 0,
		0, 0, 0, 1
	};
	fillUniformBuffer(TRANSFORM_BUFFER, &mat, sizeof(glm::mat4));
}
