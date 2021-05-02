#pragma once

namespace tge::main {

enum class Error { NONE, ALREADY_RUNNING, ALREADY_INITIALIZED, NOT_INITIALIZED, NO_GRAPHIC_QUEUE_FOUND };

extern Error error;

}
