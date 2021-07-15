#pragma once

#include "../Module.hpp"
#include "WindowModule.hpp"
#include "GameGraphicsModule.hpp"

namespace tge::gui {

class GUIModule : public tge::main::Module {

public:
  tge::graphics::WindowModule *winModule;
  tge::graphics::APILayer *api;

  void* pool;
  void* buffer;

  GUIModule(tge::graphics::WindowModule *winModule,
            tge::graphics::APILayer *api)
      : winModule(winModule), api(api) {}

  main::Error init() override;

  void tick(double deltatime) override;

  void destroy() override;
};

} // namespace tge::gui
