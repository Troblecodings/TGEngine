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

	while (true) {
		glfwPollEvents();
		if (glfwWindowShouldClose(window)) {
			break;
		}
	}

	destroyRenderPass();
	destroyDevice();
	destroyWindow();
	destroyInstance();
}