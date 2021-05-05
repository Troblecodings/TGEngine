#pragma once

#include <filesystem>
#include <stdint.h>
#include <type_traits>

namespace fs = std::filesystem;

namespace tge::util {

template <class T> concept Callable = std::is_invocable_v<T>;
template <Callable C> class OnExit {

  const C call;

public:
  OnExit(const C cin) : call(cin) {}
  ~OnExit() { call(); }
};

uint8_t *wholeFile(fs::path &path);

} // namespace tge::util