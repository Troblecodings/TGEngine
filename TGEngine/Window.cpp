#include "Window.hpp"

uint32_t x = 0, y = 0, width, height;
GLFWmonitor* monitor;
GLFWwindow* window;
VkSurfaceKHR surface;

void createWindow(nio::Properties properties) {
	glfwSetErrorCallback(ErrorCallback);
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	getMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	if (properties.getBoolean("fullscreen").rvalue) {
		glfwWindowHint(GLFW_DECORATED, VK_FALSE);
		width = mode->width;
		height = mode->height;
	}
	else if (properties.getBoolean("center").rvalue) {
		height = properties.getInt("height").rvalue;
		width = properties.getInt("width").rvalue;
		x = mode->width / 2 - width / 2;
		y = mode->height / 2 - height / 2;
	}
	else {
		height = properties.getInt("height").rvalue;
		width = properties.getInt("width").rvalue;
		x = properties.getInt("posx").rvalue;
		y = properties.getInt("posy").rvalue;
	}
	window = glfwCreateWindow(width, height, properties.getString("app_name").value, monitor, nullptr);
	glfwSetWindowPos(window, x, y);
}

void createWindowSurface() {
	if (!glfwVulkanSupported()) {
		std::cout << "Vulkan not supported in GLFW make sure you installed everything correctly!" << std::endl;
		_sleep(10000);
		exit(-1);
	}
	last_result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
	HANDEL(last_result)
}

void getMonitor() {
	monitor = glfwGetPrimaryMonitor();
}

void destroyWindow() {
	delete monitor;
	glfwDestroyWindow(window);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	glfwTerminate();
}