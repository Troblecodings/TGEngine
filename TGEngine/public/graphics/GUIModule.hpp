#pragma once

#include "../Module.hpp"

namespace tge::gui {

class GUIModule : public tge::main::Module {

public:

  void *pool;
  void *buffer;
  void *renderpass;
  void *framebuffer;

  main::Error init() override;

  void tick(double deltatime) override;

  void destroy() override;

  virtual void renderGUI() = 0;
};

} // namespace tge::gui
