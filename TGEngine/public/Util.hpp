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
  constexpr OnExit(const C cin) : call(cin) {}
  constexpr ~OnExit() { call(); }
};

std::unique_ptr<uint8_t[]> wholeFile(const fs::path &path,
                                     size_t *out = nullptr);

} // namespace tge::util