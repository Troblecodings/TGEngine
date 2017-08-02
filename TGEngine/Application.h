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
	};

	/*
	 * Creates an Application
	 */
	void createApplication(Application app);

	/*
	 * Destroys the Application
	 */
	void destroyApplictaion(Application app);
}