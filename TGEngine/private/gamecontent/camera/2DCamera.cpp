#include "../../../public/gamecontent/camera/2DCamera.hpp"
#include "../../../public/pipeline/buffer/UniformBuffer.hpp"
#include "../../../public/pipeline/window/Window.hpp"

using namespace tge::buf;

void tge::gmc::setTopDownCamera(TopDownCamera camera) {
	glm::mat4 mat =
	{ tge::win::mainWindowHeight / (float)tge::win::mainWindowWidth, 0, 0, camera.positionx,
		0, 1, 0, camera.positiony,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	fillUniformBuffer(TRANSFORM_BUFFER, &mat, sizeof(glm::mat4));
}
