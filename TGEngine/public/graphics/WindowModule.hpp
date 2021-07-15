#pragma once

#include "../Module.hpp"
#include <mutex>
#include <thread>

namespace tge::graphics {

struct WindowProperties {
  bool centered = false;   // Not implemented
  int x = 0;               // Ignored if centered
  int y = 0;               // Ignored if centered
  char fullscreenmode = 0; // Not implemented
  int width = 800;         // Ignored if fullscreenmode != 0
  int height = 800;        // Ignored if fullscreenmode != 0
};

class WindowModule : public main::Module {

public:
  void *hInstance;
  void *hWnd;
  void *customFn = nullptr;
  bool closeRequest = false;
  std::thread osThread;
  std::mutex osMutex;

  main::Error init() override;

  void tick(double deltatime) override;

  void destroy() override;

  WindowProperties getWindowProperties();
};

} // namespace tge::graphics
