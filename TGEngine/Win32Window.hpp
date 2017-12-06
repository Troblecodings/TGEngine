#pragma once
#ifdef _WIN32

#include "TGEngine.hpp"
#include "Window.hpp"

#define WINDOW_HANDLE L"TGWINDOWHANDLE"

extern LPCWCHAR lresult;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void registerWindowClass();

#endif
