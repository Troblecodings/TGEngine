#pragma once

#include "..\..\stdbase.hpp"
#include "..\Instance.hpp"
#include "..\..\IO\Properties.hpp"
#include <string>

#ifdef _WIN32

#include <Windows.h>
#include <tchar.h>
#include "Win32Window.hpp"
#include <windowsx.h>
#include "..\..\IO\Mouse.hpp"

#define TG_MAIN_WINDOW_HANDLE L"TG_MAIN_WINDOW_HANDLE"

struct Window {

	SINCE(0, 0, 3)
	Window(wchar_t* name);

	void pollevents(),
		destroy(),
		createWindowSurface();

	uint32_t x = 0, 
		y = 0, 
		width = 0, 
		height = 0;

	bool close_request = false,
		decorated = true,
		cursor = true,
		consume_input = true,
		focused = true;

	VkSurfaceKHR surface;

	HWND __impl_window;
	HCURSOR __impl_cursor;
	wchar_t* __impl_handle;

	SINCE(0, 0, 3)
	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

#define GET_SIZE(x, y) const HWND hDesktop = GetDesktopWindow();\
RECT desktop;\
GetWindowRect(hDesktop, &desktop);\
x = desktop.right;\
y = desktop.bottom;
#endif

extern uint32_t x, y, width, height, d_width, d_height;
extern VkSurfaceKHR surface;

/*
 * Creates a window based on the properties
 */
SINCE(0, 0, 1)
void createWindow(Window window, nio::Properties* properties = nullptr);

/*
 * Don't use
 */
UNFINISHED
SINCE(0, 0, 1)
void getMonitor();