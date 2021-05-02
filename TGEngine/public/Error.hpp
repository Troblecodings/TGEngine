#pragma once

namespace tge::main {

enum class Error {
  NONE,
  ALREADY_RUNNING,
  ALREADY_INITIALIZED,
  NOT_INITIALIZED,
  NO_GRAPHIC_QUEUE_FOUND,
  SURFACECREATION_FAILED,
  COULD_NOT_CREATE_WINDOW,
  COULD_NOT_CREATE_WINDOW_CLASS
};

extern Error error;

} // namespace tge::main
