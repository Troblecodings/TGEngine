#pragma once

#include "stdbase.h"

namespace Pipeline {

	struct Window {
		bool autosize;
		char* title;
		GLFWwindow* window;
		VkExtent2D size;
		VkExtent2D pos;
	};

	/*
	 * Creates a Window
	 */
	SINCE(0, 0, 6)
	void createWindow(Window* window);

	/*
	 * Destroys the Window
	 */
	SINCE(0, 0, 6)
	void destroyWindow(Window* window);
}