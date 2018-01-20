#pragma once

#ifdef _WIN32

#include "..\..\stdbase.hpp"

#define WINDOW_HANDLE L"TGWINDOWHANDLE"

struct Window {
	HWND __impl_window;

	void pollevents();

	bool close_request = false;
	bool decorated = true;
};

extern Window window;

#define GET_SIZE(x, y) const HWND hDesktop = GetDesktopWindow();\
RECT desktop;\
GetWindowRect(hDesktop, &desktop);\
x = desktop.right;\
y = desktop.bottom;

extern LPCWCHAR lresult;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void registerWindowClass();

#endif
