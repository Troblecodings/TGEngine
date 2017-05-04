#include "stdafx.h"
#include "Console.h"

struct Window {
	bool autosize;
	char* title;
	GLFWwindow* window;
	VkExtent2D size;
};

void creatWindow(Window window) {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

	if (window.autosize) {
		GLFWmonitor* mon = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(mon);
		window.size = { static_cast<uint32_t>(mode->width),static_cast<uint32_t>(mode->height) };
	}
	window.window = glfwCreateWindow(window.size.width, window.size.height, window.title, nullptr, nullptr);
	glfwSetWindowPos(window.window, 0, 0);
};

void destroyWindow(Window window) {
	glfwDestroyWindow(window.window);
	glfwTerminate();
}

namespace WINDOW{ Window window; }
	