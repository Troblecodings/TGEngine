#include "../public/TGEngine.hpp"
#include "../public/graphics/VulkanGraphicsModule.hpp"

namespace tge::main {

std::vector<Module *> modules;
bool exitRequest = false;
bool isRunning = false;
bool isInitialized = false;

graphics::APILayer *usedApiLayer = nullptr;
graphics::GameGraphicsModule *gameModule = nullptr;
graphics::WindowModule *winModule = nullptr;

Error init() {
  if (isInitialized)
    return error = Error::ALREADY_INITIALIZED;
  winModule = new graphics::WindowModule();
  modules.push_back(winModule);
  usedApiLayer = graphics::getNewVulkanModule();
  modules.push_back(usedApiLayer);
  gameModule = new graphics::GameGraphicsModule(usedApiLayer, winModule);
  usedApiLayer->setGameGraphicsModule(gameModule);
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

    if (exitRequest || winModule->closeRequest)
      break;
    auto endpoint = steady_clock::now();
    deltatime = duration_cast<duration<double>>(endpoint - startpoint).count();
    startpoint = endpoint;
  }
  for (auto mod : modules) {
    mod->destroy();
    delete mod;
  }
  modules.clear();
  usedApiLayer = nullptr;
  gameModule = nullptr;
  winModule = nullptr;
  isRunning = false;
  isInitialized = false;
  exitRequest = false;
  return error = Error::NONE;
}

void requestExit() { exitRequest = true; }

Error lastError() { return error; }

graphics::APILayer *getAPILayer() { return usedApiLayer; }

graphics::GameGraphicsModule *getGameGraphicsModule() { return gameModule; }

} // namespace tge::main