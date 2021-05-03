#include "../public/Util.hpp"
#include <fstream>

namespace tge::util {

uint8_t *wholeFile(fs::path &path) {
  std::ifstream inputstream(path,
                            std::ios::ate | std::ios::in | std::ios::binary);
  if (!inputstream)
    return nullptr;
  const auto size = inputstream.tellg();
  inputstream.seekg(0, SEEK_SET);
  uint8_t *fileData = new uint8_t[(size_t)size + 1];
  inputstream.read((char *)fileData, size);
  fileData[size] = 0;
  return fileData;
}

} // namespace tge::util