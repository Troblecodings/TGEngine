#pragma once

#include <chrono>
#include <vector>
#include "Module.hpp"
#include "graphics/GraphicsModule.hpp"
#include "Error.hpp"

namespace tge::main {

extern std::vector<Module*> modules;

Error init();

Error start();

void requestExit();

Error lastError();

} // namespace tge::main
