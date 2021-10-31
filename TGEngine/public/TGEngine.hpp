#pragma once

#include "Error.hpp"
#include "Module.hpp"
#include "graphics/GameGraphicsModule.hpp"

#include <chrono>
#include <vector>

namespace tge::main {

extern std::vector<Module *> lateModules;

Error init();

Error start();

Error lastError();

graphics::APILayer *getAPILayer();

graphics::GameGraphicsModule *getGameGraphicsModule();

} // namespace tge::main
