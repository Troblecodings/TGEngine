#pragma once

#include "stdbase.h"
#include "Debug.h"
#include "Window.h"

namespace Pipeline {

	using namespace std;

	struct Application {
		uint32_t version;
		vector<char*> layers_to_enable;
		VkInstance* instance;
		VkSurfaceKHR* KHR;
		Window* window;
		VkAllocationCallbacks* allocator;
	};

	/*
	 * Creates an Application
	 */
	SINCE(0, 0, 6)
	void createApplication(Application* app);

	/*
	 * Destroys the Application
	 */
	SINCE(0, 0, 6)
	void destroyApplictaion(Application* app);
}