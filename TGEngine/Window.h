#pragma once

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
	void creatWindow(Window window);

	/*
	 * Destroys the Window
	 */
	void destroyWindow(Window window);
}