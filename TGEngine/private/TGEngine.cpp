#include "../public/TGEngine.hpp"
#include "../public/graphics/VulkanGraphicsModule.hpp"

namespace tge::main {

std::vector<Module *> modules;
bool exitRequest = false;
bool isRunning = false;
bool isInitialized = false;

graphics::APILayer *usedApiLayer;
graphics::GameGraphicsModule *gameModule;

Error init() {
  if (isInitialized)
    return error = Error::ALREADY_INITIALIZED;
  gameModule = new graphics::GameGraphicsModule(graphics::getNewVulkanModule);
  usedApiLayer = gameModule->getAPILayer();
  modules.push_back(usedApiLayer);
  modules.push_back(gameModule);
  for (auto mod : modules) {
    error = mod->init();
    if (error != Error::NONE)
      return error;
  }
  isInitialized = true;
  return error = Error::NONE;
}

Error start() {
  if (!isInitialized)
    return error = Error::NOT_INITIALIZED;
  if (isRunning)
    return error = Error::ALREADY_RUNNING;

  using namespace std::chrono;
  auto startpoint = steady_clock::now();
  double deltatime = 0;
  isRunning = true;
  for (;;) {
    for (auto mod : modules)
      mod->tick(deltatime);

    if (exitRequest)
      break;
    auto endpoint = steady_clock::now();
    deltatime = duration_cast<duration<double>>(endpoint - startpoint).count();
    startpoint = endpoint;
  }
  for (auto mod : modules) {
    mod->destroy();
    delete mod;
  }
  isRunning = false;
  isInitialized = false;
  return error = Error::NONE;
}

void requestExit() { exitRequest = true; }

Error lastError() { return error; }

graphics::APILayer *getAPILayer() { return usedApiLayer; }

} // namespace tge::main