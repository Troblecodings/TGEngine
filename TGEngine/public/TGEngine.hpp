#pragma once

#include "Error.hpp"
#include "Module.hpp"
#include "graphics/GameGraphicsModule.hpp"
#include <chrono>
#include <vector>

namespace tge::main {

extern std::vector<Module *> modules;

Error init();

Error start();

void requestExit();

Error lastError();

graphics::APILayer *getAPILayer();

} // namespace tge::main
