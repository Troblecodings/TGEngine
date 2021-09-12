#include "..\..\public\graphics\WindowModule.hpp"
#include <iostream>
#include <string>
#include "..\..\public\Util.hpp"

#ifdef WIN32
#include <Windows.h>
#endif

namespace tge::graphics {

#ifdef WIN32
LRESULT CALLBACK callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
  if (Msg == WM_CLOSE) {
    util::requestExit();
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

main::Error windowsInit(WindowModule *winModule) {
  HMODULE systemHandle = GetModuleHandle(nullptr);
  if (!systemHandle)
    return main::Error::NO_MODULE_HANDLE;
  winModule->hInstance = systemHandle;

  const auto windowProperties = winModule->getWindowProperties();

  WNDCLASSEX wndclass;
  FillMemory(&wndclass, sizeof(WNDCLASSEX), 0);
  wndclass.cbSize = sizeof(WNDCLASSEX);
  wndclass.style = CS_ENABLE | CS_OWNDC | CS_HREDRAW;
  wndclass.lpfnWndProc = callback;
  wndclass.hInstance = systemHandle;
  wndclass.lpszClassName = ENGINE_NAME;

  auto regWndClass = RegisterClassEx(&wndclass);
  if (!regWndClass) {
    if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
      return main::Error::COULD_NOT_CREATE_WINDOW_CLASS;
  }

  auto window = CreateWindowEx(
      WS_EX_APPWINDOW, ENGINE_NAME, APPLICATION_NAME,
      WS_CLIPSIBLINGS | WS_CAPTION | WS_SIZEBOX | WS_SYSMENU,
      windowProperties.x, windowProperties.y, windowProperties.width,
      windowProperties.height, NULL, NULL, systemHandle, NULL);
  if (!window)
    return main::Error::COULD_NOT_CREATE_WINDOW;
  winModule->hWnd = window;
  ShowWindow(window, SW_SHOW);
  UpdateWindow(window);
  return main::Error::NONE;
}

void windowsPoolMessages(WindowModule *winModule) {
  MSG msg;
  const HWND wnd = (HWND)winModule->hWnd;
  while (PeekMessage(&msg, wnd, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    if (winModule->customFn != nullptr)
      ((WNDPROC)winModule->customFn)(wnd, msg.message, msg.wParam, msg.lParam);
  }
}

void windowsDestroy(WindowModule *winModule) {
  DestroyWindow((HWND)winModule->hWnd);
}

#endif // WIN32

main::Error WindowModule::init() {
#ifdef WIN32
  osThread = std::thread([winM = this] {
    winM->osMutex.lock();
    windowsInit(winM);
    winM->osMutex.unlock();
    std::lock_guard lg2(winM->exitMutex);
    while (!winM->closing) {
      windowsPoolMessages(winM);
    }
    windowsDestroy(winM);
  });
  osThread.detach();
  std::lock_guard lg(this->osMutex);
#endif // WIN32
  return main::Error::NONE;
}

void WindowModule::tick(double deltatime) {}

void WindowModule::destroy() {
  this->closing = true;
  std::lock_guard lg(this->exitMutex);
}

WindowProperties WindowModule::getWindowProperties() {
  return WindowProperties();
}

} // namespace tge::graphics
