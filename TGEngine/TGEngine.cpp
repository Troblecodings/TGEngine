#include "TGEngine.hpp"
#include "Pipeline.hpp"

static nio::Properties properties;

void initTGEngine() {
	nio::readProperties("Properties.xml", &properties);
	createWindow(properties);
	createInstance(properties, { "VK_LAYER_LUNARG_parameter_validation" }, { });
	createWindowSurface();

	while (true) {
		glfwPollEvents();
		if (glfwWindowShouldClose(window)) {
			break;
		}
	}

	destroyWindow();
	destroyInstance();
}