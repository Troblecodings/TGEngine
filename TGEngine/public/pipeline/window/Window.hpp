#pragma once

#include "../../../public/Stdbase.hpp"
#include "../../../public/pipeline/Instance.hpp"
#include <string>
#include "../../../public/io/Mouse.hpp"
#include "../../../public/io/Keyboard.hpp"
#include "../../../public/gamecontent/PlayerController.hpp"

namespace tge::win {

	extern uint8_t states;
	/*
	 * This is the current vulkan surface
	 * We draw on
	 */
	extern VkSurfaceKHR windowSurface;

	extern uint32_t mainWindowWidth;
	extern uint32_t mainWindowHeight;
	extern uint32_t mainWindowX;
	extern uint32_t mainWindowY;

	extern float mouseX;
	extern float mouseY;
	extern float mouseHomogeneousX;
	extern float mouseHomogeneousY;

	extern bool isDecorated;
	extern bool isFullscreen;
	extern bool isConsumingInput;
	extern bool isCloseRequested;
	extern bool isFocused;
	extern bool isMinimized;

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#include <tchar.h>
#include <windowsx.h>

#ifdef UNICODE
#define TG_MAIN_WINDOW_HANDLE L"TGHANDLE"
#else
#define TG_MAIN_WINDOW_HANDLE "TGHANDLE"
#endif

	extern HMODULE systemModule;
	extern HWND winHWND;
	extern HCURSOR winHCURSOR;

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/*
	 * Creates the actual window
	 * Attention! Much platform code here!
	 */
	void createWindowClass();
#endif

	/*
	 * Creates the main window
	 */
	void createWindow();

	/*
	 * Destroys all windows
	 */
	void destroyWindows();

	/*
	 * Create surfaces for each created window
	 */
	void createWindowSurfaces();

	/*
	 * Polls all the events from the window given by the OS API
	 */
	void pollEvents();

}
