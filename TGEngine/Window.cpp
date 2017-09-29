#include "Window.hpp"

uint32_t x = 0, y = 0, width = 0, height = 0;
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
	last_result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
	HANDEL(last_result)
}

void getMonitor() {
	monitor = glfwGetPrimaryMonitor();
}

void destroyWindow() {
	glfwDestroyWindow(window);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	glfwTerminate();
}