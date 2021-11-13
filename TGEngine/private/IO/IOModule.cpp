#include "../../public/IO/IOModule.hpp"
#include "../../public/TGEngine.hpp"
#include <Windows.h>
#include <iostream>
#include <windowsx.h>

namespace tge::io {

std::vector<IOModule *> ios;

#ifdef WIN32
LRESULT CALLBACK callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
  switch (Msg) {
  case WM_MOUSEMOVE: {
    const auto xParam = GET_X_LPARAM(lParam);
    const auto yParam = GET_Y_LPARAM(lParam);
    for (const auto io : ios)
      io->mouseEvent({xParam, yParam, 0});
  } break;
  case WM_KEYDOWN: {
    for (const auto io : ios)
      io->keyboardEvent({(uint32_t)wParam});
  } break;
  default:
    break;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
#endif

bool funcAdded = false;

main::Error IOModule::init() {
  if (!funcAdded) {
    auto win = main::getGameGraphicsModule()->getWindowModule();
    win->customFn.push_back((void *)callback);
    funcAdded = true;
  }
  ios.push_back(this);
  return main::Error::NONE;
}

}; // namespace tge::io
