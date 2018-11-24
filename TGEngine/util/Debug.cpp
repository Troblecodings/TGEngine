#pragma once

#include "Debug.hpp"

using namespace std;

void printVersion(int version) {
	cout << VK_VERSION_MAJOR(version) << "." << VK_VERSION_MINOR(version) << "." << VK_VERSION_PATCH(version);
}