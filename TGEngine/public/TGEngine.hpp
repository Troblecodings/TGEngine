#pragma once

#include <chrono>
#include <vector>
#include "Error.hpp"
#include "Module.hpp"

namespace tge::main {

extern std::vector<Module*> modules;

Error init();

Error start();

void requestExit();

Error lastError();

} // namespace tge::main
