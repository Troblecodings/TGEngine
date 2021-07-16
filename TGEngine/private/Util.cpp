#include "../public/Util.hpp"
#include <fstream>

namespace tge::util {

bool exitRequest = false;

std::vector<char> wholeFile(const fs::path &path) {
  std::ifstream inputstream(path,
                            std::ios::ate | std::ios::in | std::ios::binary);
  if (!inputstream)
    return {};
  const size_t size = (size_t)inputstream.tellg();
  inputstream.seekg(0, SEEK_SET);
  std::vector<char> fileData(size + 1);
  inputstream.read((char *)fileData.data(), size);
  fileData[size] = 0;
  return fileData;
}

void requestExit() { exitRequest = true; }

} // namespace tge::util