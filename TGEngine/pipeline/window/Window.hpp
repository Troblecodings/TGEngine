#pragma once

#include "../../Stdbase.hpp"
#include "../Instance.hpp"
#include <string>
#include "../../io/Mouse.hpp"
#include "../../io/Keyboard.hpp"

#if defined(_WIN32) || defined(_WIN64)
#ifndef _WIN_H_
#define _WIN_H_
#include <Windows.h>
#include <tchar.h>
#include <windowsx.h>
#endif // !_WIN_H_
#define TG_MAIN_WINDOW_HANDLE L"TGHANDLE"

extern HMODULE sys_module;
extern std::vector<HWND> __impl_window_list;

struct Window {

	void pollevents();

	int x = 0,
		y = 0,
		width = 0,
		height = 0;

	long middleX = 0, middleY = 0;

	bool close_request = false,
		decorated = true,
		cursor = true,
		consume_input = true,
		focused = true,
		minimized = false;

	VkSurfaceKHR surface;

	HWND __impl_window;
	HCURSOR __impl_cursor;
};

#define GET_SIZE(x, y) const HWND hDesktop = GetDesktopWindow();\
RECT desktop;\
GetWindowRect(hDesktop, &desktop);\
x = desktop.right;\
y = desktop.bottom;


SINCE(0, 0, 3)
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

extern uint32_t d_width, d_height;
extern std::vector<Window*> window_list;

/*
 * Creates a window based on the properties
 */
SINCE(0, 0, 1)
void createWindow(Window* window);

/*
 * Creates the actual window
 * Attention! Much platform code here!
 */
SINCE(0, 0, 3)
void createWindowClass();

/*
 * Destroys all windows
 */
SINCE(0, 0, 4)
void destroyWindows();

/*
 * Create surfaces for each created window
 */
SINCE(0, 0, 4)
void createWindowSurfaces();

/*
 * Don't use
 */
UNFINISHED
SINCE(0, 0, 1)
void getMonitor();