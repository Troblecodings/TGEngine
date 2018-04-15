#pragma once

#ifdef _WIN32

#include "..\..\stdbase.hpp"
#include <windowsx.h>
#include "..\..\IO\Mouse.hpp"

#define WINDOW_HANDLE L"TGWINDOWHANDLE"

struct Window {
	HWND __impl_window;

	void pollevents();

	bool close_request = false;
	bool decorated = true;
	bool cursor = true;
};

extern Window window;
extern HCURSOR cursor;
extern LPCWCHAR lresult;

#define GET_SIZE(x, y) const HWND hDesktop = GetDesktopWindow();\
RECT desktop;\
GetWindowRect(hDesktop, &desktop);\
x = desktop.right;\
y = desktop.bottom;


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void registerWindowClass();

#endif
