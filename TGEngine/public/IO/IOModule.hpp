#pragma once

#include "../Module.hpp"

namespace tge::io {

struct KeyboardEvent {
  int signal;
};

struct MouseEvent {
  int x;
  int y;
  int pressed;
};

class IOModule : public tge::main::Module {

  main::Error init() override;

public:
  virtual void mouseEvent(const MouseEvent event) = 0;

  virtual void keyboardEvent(const KeyboardEvent event) = 0;
};

} // namespace tge::io
