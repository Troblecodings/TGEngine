#include "..\..\public\graphics\WindowModule.hpp"
#include <iostream>
#include <string>

#ifdef WIN32
#include <Windows.h>
#endif

namespace tge::graphics {

#ifdef WIN32
LRESULT CALLBACK callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
  printf("%d", Msg);
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
#endif // WIN32

main::Error WindowModule::init() {

#ifdef WIN32
  HMODULE systemHandle = GetModuleHandle(nullptr);
  if (!systemHandle)
    return main::Error::NO_MODULE_HANDLE;
  this->hInstance = systemHandle;

  const auto windowProperties = this->getWindowProperties();

  WNDCLASSEX wndclass;
  FillMemory(&wndclass, sizeof(WNDCLASSEX), 0);
  wndclass.cbSize = sizeof(WNDCLASSEX);
  wndclass.style = CS_ENABLE | CS_OWNDC | CS_HREDRAW;
  wndclass.lpfnWndProc = callback;
  wndclass.hInstance = systemHandle;
  wndclass.lpszClassName = ENGINE_NAME;

  LPCSTR regWndClass = (LPCSTR)RegisterClassEx(&wndclass);
  if (!regWndClass) {
    if (GetLastError() == ERROR_CLASS_ALREADY_EXISTS)
      regWndClass = ENGINE_NAME;
    else
      return main::Error::COULD_NOT_CREATE_WINDOW_CLASS;
  }

  auto window = CreateWindowEx(
      WS_EX_APPWINDOW, regWndClass, APPLICATION_NAME,
      WS_CLIPSIBLINGS | WS_CAPTION | WS_SIZEBOX | WS_SYSMENU,
      windowProperties.x, windowProperties.y, windowProperties.width,
      windowProperties.height, NULL, NULL, systemHandle, NULL);
  if (!window)
    return main::Error::COULD_NOT_CREATE_WINDOW;
  this->hWnd = window;
  ShowWindow(window, SW_SHOW);
  UpdateWindow(window);
#endif // WIN32

  return main::Error::NONE;
}

void WindowModule::tick(double deltatime) {
#ifdef WIN32
  MSG msg;
  const HWND wnd = (HWND)this->hWnd;
  while (PeekMessage(&msg, wnd, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  printf("Tickdone");
#endif
}

void WindowModule::destroy() {
#ifdef WIN32
  ShowWindow((HWND)this->hWnd, SW_HIDE);
  if(!DestroyWindow((HWND)this->hWnd))
    throw std::runtime_error(std::to_string(GetLastError()));
#endif
}

WindowProperties WindowModule::getWindowProperties() {
  return WindowProperties();
}

} // namespace tge::graphics
