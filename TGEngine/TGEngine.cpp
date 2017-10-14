#include "TGEngine.hpp"
#include "Pipeline.hpp"

nio::Properties properties;

void initTGEngine() {
	nio::readProperties("Properties.xml", &properties);
	createWindow(properties);
	createInstance(properties, { "VK_LAYER_LUNARG_standard_validation" }, { });
	createWindowSurface();
	createDevice({}, {});
	createRenderpass();
	createShader("vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
	createShader("frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
	createShaderInput(0, offsetof(Vertex, position), VK_FORMAT_R32G32_SFLOAT);
	createShaderInput(1, offsetof(Vertex, color), VK_FORMAT_R32G32B32_SFLOAT);
	createPipeline();

	while (true) {
		glfwPollEvents();
		if (glfwWindowShouldClose(window)) {
			break;
		}
	}
	destroyPipeline();
	destroyShaders();
	destroyRenderPass();
	destroyDevice();
	destroyWindow();
	destroyInstance();
}