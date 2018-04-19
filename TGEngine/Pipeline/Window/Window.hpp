#pragma once

#include "..\..\stdbase.hpp"
#include "..\Instance.hpp"
#include "..\..\IO\Properties.hpp"
#include <string>
#include "..\..\IO\Mouse.hpp"

#ifdef _WIN32

#include <Windows.h>
#include <tchar.h>
#include <windowsx.h>

#define TG_MAIN_WINDOW_HANDLE L"TGHANDLE"

struct Window {

	SINCE(0, 0, 3)
	Window(wchar_t* name);

	void pollevents(),
		destroy(),
		createWindowSurface();

	int x = 0, 
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
};

#define GET_SIZE(x, y) const HWND hDesktop = GetDesktopWindow();\
RECT desktop;\
GetWindowRect(hDesktop, &desktop);\
x = desktop.right;\
y = desktop.bottom;
#endif

extern uint32_t d_width, d_height;
extern std::vector<Window*> window_list;
extern HMODULE sys_module;

/*
 * Creates a window based on the properties
 */
SINCE(0, 0, 1)
void createWindow(Window* window, nio::Properties* properties = nullptr);

SINCE(0, 0, 3)
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

SINCE(0, 0, 3)
void createWindowClass();

/*
 * Don't use
 */
UNFINISHED
SINCE(0, 0, 1)
void getMonitor();