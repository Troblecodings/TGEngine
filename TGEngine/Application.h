#pragma once

#include "stdbase.h"
#include "Debug.h"

namespace Pipeline {

	using namespace std;

	struct Application {
		char* title;
		uint32_t version;
		vector<char*> layers_to_enable;
		VkInstance* instance;
		VkSurfaceKHR* KHR;
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