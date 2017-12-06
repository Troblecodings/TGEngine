#pragma once

#include "Debug.hpp"

using namespace std;

void printVersion(int version) {
	cout << VK_VERSION_MAJOR(version) << "." << VK_VERSION_MINOR(version) << "." << VK_VERSION_PATCH(version);
}

void ErrorCallback(int, const char* err_str)
{
	std::cout << "GLFW Error: " << err_str << std::endl;
}

void error(char* errorname, int errorcode) {
	cout << "Error occured (" << errorcode << ")" << endl << errorname << endl;
	_sleep(100000);
	exit(errorcode);
}