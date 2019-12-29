#pragma once

#include "../../Stdbase.hpp"
#include "../Instance.hpp"
#include <string>
#include "../../io/Mouse.hpp"
#include "../../io/Keyboard.hpp"
#include "../../gamecontent/PlayerController.hpp"

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
extern unsigned char states;

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


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

extern uint32_t d_width, d_height;
extern std::vector<Window*> windowList;

/*
 * Creates a window with the constraints given
 * by the read tgeproperties, therfore this is
 * defined by the according xml file
 * See Stdbase.hpp
 *   -> extern prop::Properties* tgeproperties;
 */
void createWindow(Window* window);

/*
 * Creates the actual window
 * Attention! Much platform code here!
 */
void createWindowClass();

/*
 * Destroys all windows
 */
void destroyWindows();

/*
 * Create surfaces for each created window
 */
void createWindowSurfaces();

/*
 * Don't use
 */
void getMonitor();