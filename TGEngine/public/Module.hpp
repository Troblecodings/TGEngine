#pragma once

namespace tge::main {

class Module {
public:
  virtual void init() {}

  virtual void tick(double deltatime) {}

  virtual void destroy() {}
};

} // namespace tge::main