#pragma once

#include "Error.hpp"

namespace tge::main {

class Module {
public:
  virtual ~Module() {}

  virtual Error init() { return Error::NONE; }

  virtual void tick(double deltatime) {}

  virtual void destroy() {}
};

} // namespace tge::main