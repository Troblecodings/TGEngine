#pragma once

#define DEBUG
#include <stdio.h>
#include <iostream>
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan\vulkan.h>
#include <vector>
#include <string>
#include "Annotations.hpp"
#include <thread>
#include "Properties.hpp"
#include "Math.hpp"
#include "Vertex.hpp"
