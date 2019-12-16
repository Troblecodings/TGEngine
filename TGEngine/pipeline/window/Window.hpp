#pragma once

#include "../../Stdbase.hpp"
#include "../Instance.hpp"
#include <string>
#include "../../io/Mouse.hpp"
#include "../../io/Keyboard.hpp"
#include "../../gamecontent/PlayerController.hpp"

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

    extern bool isDecoreted;
    extern bool isFullscreeen;
    extern bool isConsumingInput;
    extern bool isCloseRequested;
    extern bool isFocused;
    extern bool isMinimized;

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#include <tchar.h>
#include <windowsx.h>
#define TG_MAIN_WINDOW_HANDLE L"TGHANDLE"

    extern HMODULE systemModule;
    extern HWND winHWND;
    extern HCURSOR winHCURSOR;

    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

    /*
     * Creates the main window
     */
    void createWindow();

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
     * Polls all the events from the window given by the OS API
     */
    void pollEvents();

}