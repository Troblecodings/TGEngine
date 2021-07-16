#pragma once

#include "Error.hpp"
#include "Module.hpp"
#include "graphics/GameGraphicsModule.hpp"
#include "graphics/GUIModule.hpp"

#include <chrono>
#include <vector>

namespace tge::main {

extern std::vector<Module *> modules;

Error init();

Error start();

Error lastError();

graphics::APILayer *getAPILayer();

graphics::GameGraphicsModule *getGameGraphicsModule();

gui::GUIModule *getGUIModule();

} // namespace tge::main
