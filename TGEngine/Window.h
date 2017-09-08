#pragma once

#include "stdbase.h"
#include "Properties.h"
#include "Debug.h"

namespace Pipeline {

	struct Window {
		char* title;
		nio::Properties* properties;
		VkExtent2D size;
		VkExtent2D pos;
		GLFWwindow* window;
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