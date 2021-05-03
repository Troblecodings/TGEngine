#pragma once

#include <stdint.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace tge::util {

uint8_t *wholeFile(fs::path &path);

}