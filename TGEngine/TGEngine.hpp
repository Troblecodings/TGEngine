#pragma once

#include <stdio.h>
#include <iostream>
#include <vulkan\vulkan.h>
#include <GLFW\glfw3.h>
#include <vector>
#include <string>
#include "Annotations.hpp"
#include <thread>
#include "Properties.hpp"

extern nio::Properties properties;
extern uint32_t image_count;

SINCE(0, 0, 1)
void initTGEngine();

/*
* Handels a vulkan error
*/
SINCE(0, 0, 1)
#define HANDEL(result)\
if (result != VK_SUCCESS) {\
std::cout << "FAILED WITH ERROR: " << result << std::endl;\
_sleep(10000);\
exit(result);\
}
