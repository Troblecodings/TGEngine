#pragma once

#include "../GameGraphicsModule.hpp"
#include <vector>
#include <array>
#include <string>

#ifndef APPLICATION_NAME
#define APPLICATION_NAME "unknown"
#endif

#ifndef APPLICATION_VERSION
#define APPLICATION_VERSION VK_MAKE_VERSION(1, 0, 0)
#endif

namespace tge::graphics {

APILayer *getNewVulkanModule();

} // namespace tge::graphics
