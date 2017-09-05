#pragma once

#include "Window.h"

namespace Pipeline {

	void createWindow(Window* window) {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

		/*
		 * TODO Find best Monitor
		 * and make it selectable via @Properties
		 */
		GLFWmonitor* mon = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(mon);
		bool autosize = window->properties->getBoolean("fullscreen").rvalue;
		if (autosize) {
			window->size = { static_cast<uint32_t>(mode->width),  static_cast<uint32_t>(mode->height) };
			window->pos = {0, 0};
		}
		else {
			window->size = { static_cast<uint32_t>(window->properties->getInt("width").rvalue), static_cast<uint32_t>(window->properties->getInt("height").rvalue)};
		}
		window->title = window->properties->getString("app_name").value;
		window->window = glfwCreateWindow(window->size.width, window->size.height, window->title, nullptr, nullptr);
		if (window->properties->getInt("height").rvalue && !autosize) {
			window->pos = { static_cast<uint32_t>((mode->width / 2) - (window->size.width / 2)), static_cast<uint32_t>((mode->height / 2) - (window->size.height / 2)) };
		}
		else if(!autosize){
			window->pos = { static_cast<uint32_t>(window->properties->getInt("posx").rvalue), static_cast<uint32_t>(window->properties->getInt("posx").rvalue) };
		}
		glfwSetWindowPos(window->window, window->pos.width, window->pos.height);
	};

	void destroyWindow(Window* window) {
		glfwDestroyWindow(window->window);
		glfwTerminate();
	}
}