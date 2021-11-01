#include "../../public/IO/IOModule.hpp"
#include "../../public/TGEngine.hpp"
#include <windowsx.h>
#include <Windows.h>
#include <iostream>

namespace tge::io {

std::vector<IOModule *> ios;

#ifdef WIN32
LRESULT CALLBACK callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
  //std::cout << "Msg: " << std::hex << Msg << std::endl;
  if (WM_MOUSEMOVE) {
    const auto xParam = GET_X_LPARAM(lParam);
    const auto yParam = GET_Y_LPARAM(lParam);
    for (const auto io : ios)
      io->mouseEvent({xParam, yParam, 0});
  } else if (WM_KEYDOWN) {
    const auto xParam = GET_X_LPARAM(lParam);
    const auto yParam = GET_Y_LPARAM(lParam);
    for (const auto io : ios)
      io->mouseEvent({xParam, yParam, 1});
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
