#include "../public/TGEngine.hpp"

namespace tge::main {

std::vector<Module *> modules;
bool exitRequest = false;
bool isRunning = false;
bool isInitialized = false;
Error error = Error::NONE;

Error init() {
  if (isInitialized)
    return error = Error::ALREADY_INITIALIZED;
  isInitialized = true;
  modules.push_back(new graphics::GraphicsModule());
  for (auto mod : modules)
    mod->init();
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

} // namespace tge::main