#pragma once

#include "stdbase.h"

namespace Pipeline {

	struct Window {
		bool autosize;
		char* title;
		GLFWwindow* window;
		VkExtent2D size;
	};

	/*
	 * Creates a Window
	 */
	void createWindow(Window window);

	/*
	 * Destroys the Window
	 */
	void destroyWindow(Window window);
}