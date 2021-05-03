#pragma once

#include "../../public/Module.hpp"

#ifndef APPLICATION_NAME
#define APPLICATION_NAME "unknown"
#endif

#ifndef APPLICATION_VERSION
#define APPLICATION_VERSION VK_MAKE_VERSION(1, 0, 0)
#endif

namespace tge::graphics {

main::Module *getNewModule();

} // namespace tge::graphics
