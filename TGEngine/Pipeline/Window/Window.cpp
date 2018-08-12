#include "Window.hpp"

uint32_t d_width = 0, d_height = 0;
std::vector<Window*> window_list;
HMODULE sys_module;

#ifdef _WIN32
Window::Window(wchar_t* name) {
	this->__impl_cursor = this->cursor ? LoadCursor(nullptr, IDC_ARROW) : NULL;
	this->__impl_handle = name;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	//TDOD performance ?
	Window* a_window = nullptr;
	for each (Window* windowptr in window_list) {
		if (windowptr->__impl_window == hwnd) {
			a_window = windowptr;
			break;
		}
	}
	if (a_window == nullptr) {
		if (msg == WM_CREATE || msg == WM_NCCREATE || msg == WM_ENABLE || msg == WM_NCPAINT || msg == WM_ERASEBKGND || msg == WM_SHOWWINDOW || msg == WM_IME_SETCONTEXT || msg == WM_IME_NOTIFY || msg == WM_GETMINMAXINFO || msg == WM_GETICON || msg == WM_NCCALCSIZE || msg == WM_ACTIVATEAPP || msg == WM_NCACTIVATE || msg == WM_ACTIVATE) {
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		return NULL;
	}

	if (msg == WM_QUIT || msg == WM_CLOSE || msg == WM_DESTROY) {
		a_window->close_request = true;
		return NULL;
	}
	else {
		if (msg == WM_MOUSELEAVE || msg == WM_NCMOUSELEAVE) {
			SetCursor(a_window->__impl_cursor);
		}
		else if (msg == WM_MOUSEMOVE) {
			inputupdate({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
			LPRECT rect = new RECT();
			GetWindowRect(hwnd, rect);
			ClipCursor(rect);
		}
		else if (msg == WM_LBUTTONDOWN) {
			FIRST_MOUSE_BUTTON = true;
		}
		else if (msg == WM_LBUTTONUP) {
			FIRST_MOUSE_BUTTON = false;
		}
		else if (msg == WM_MBUTTONDOWN) {
			THIRED_MOUSE_BUTTON = true;
		}
		else if (msg == WM_MBUTTONUP) {
			THIRED_MOUSE_BUTTON = false;
		}
		else if (msg == WM_RBUTTONDOWN) {
			SECOND_MOUSE_BUTTON = true;
		}
		else if (msg == WM_RBUTTONUP) {
			SECOND_MOUSE_BUTTON = false;
		} 
		else if (msg == WM_KILLFOCUS) {
			a_window->focused = false;
		}
		else if (msg == WM_SETFOCUS) {
			a_window->focused = true;
		}
		else if (msg == WM_SYSCOMMAND) {
			if (wParam == SC_MINIMIZE) {
				a_window->minimized = true;
			}
			else if (wParam == SC_RESTORE) {
				a_window->minimized = false;
			}
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
#endif

void setWindowProperties(Window* window, nio::Properties* properties) {
	if (properties != nullptr) {
		bool fullscreen = properties->getBoolean("fullscreen").rvalue;
		window->decorated = fullscreen ? false : properties->getBoolean("decorated").rvalue;
		window->cursor = properties->getBoolean("cursor").rvalue;
		if (fullscreen) {
			GET_SIZE(d_width, d_height)
				window->width = d_width;
			window->height = d_height;
			window->x = d_width / 2 - window->width / 2;
			window->y = d_height / 2 - window->height / 2;
		}
		else if (properties->getBoolean("center").rvalue) {
			GET_SIZE(d_width, d_height)
				window->height = properties->getInt("height").rvalue;
			window->width = properties->getInt("width").rvalue;
			window->x = d_width / 2 - window->width / 2;
			window->y = d_height / 2 - window->height / 2;
		}
		else {
			window->height = properties->getInt("height").rvalue;
			window->width = properties->getInt("width").rvalue;
			window->x = properties->getInt("posx").rvalue;
			window->y = properties->getInt("posy").rvalue;
		}
	}
}

void createWindow(Window* window, nio::Properties* properties) {
	// IMPL getMonitor();

	setWindowProperties(window, properties);

    #ifdef _WIN32 //Windows window createion
	if (window->decorated) {
		//Char unicode conversation
		if (properties != nullptr) {
			char* ch = properties->getString("app_name").value;
			const size_t cSize = strlen(ch) + 1;
			std::wstring wc(cSize, L'#');
			mbstowcs(&wc[0], ch, cSize);
			window->__impl_window = CreateWindowEx(WS_EX_APPWINDOW, TG_MAIN_WINDOW_HANDLE, (LPCWCHAR)wc.data(), WS_CLIPSIBLINGS | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, window->x, window->y, window->width + 16, window->height + 39, nullptr, nullptr, sys_module, nullptr);
		}
		else {
			window->__impl_window = CreateWindowEx(WS_EX_APPWINDOW, TG_MAIN_WINDOW_HANDLE, nullptr, WS_CLIPSIBLINGS | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, window->x, window->y, window->width + 16, window->height + 39, nullptr, nullptr, sys_module, nullptr);
		}
	}
	else {
		window->__impl_window = CreateWindowEx(WS_EX_LEFT, TG_MAIN_WINDOW_HANDLE, nullptr, WS_POPUP | WS_VISIBLE | WS_SYSMENU, window->x, window->y, window->width, window->height, nullptr, nullptr, sys_module , nullptr);
	}
#ifdef DEBUG
	if (window->__impl_window == NULL) {
		MessageBox(NULL, L"Window creation failed, sorry!", L"ERROR!", MB_ICONERROR | MB_OK);
		return;
	}
#endif // DEBUG

	ShowWindow(window->__impl_window, SW_SHOW);
	UpdateWindow(window->__impl_window);

	if (window->consume_input) {
		LPRECT win_rect = new RECT();
		GetWindowRect(window->__impl_window, win_rect);
		SetCursorPos((int)(win_rect->right - win_rect->left) / 2 + win_rect->left, (int)(win_rect->bottom - win_rect->top) / 2 + win_rect->top);
	}

	SetCursor(window->__impl_cursor);
    #endif
}

void Window::createWindowSurface() {
    #ifdef _WIN32 
	VkWin32SurfaceCreateInfoKHR surface_create_info = {
		VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
	    nullptr,
	    0,
		sys_module,
	    this->__impl_window
	};
	last_result = vkCreateWin32SurfaceKHR(instance, &surface_create_info, nullptr, &this->surface);
	HANDEL(last_result)
    #endif 
}

void createWindowClass() {
#ifdef DEBUG
	if (!
#endif // DEBUG
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_PIN, nullptr, &sys_module)
#ifndef DEBUG
		;
#endif
#ifdef DEBUG
		) {
		MessageBox(NULL, L"Can't get module, sorry!", L"ERROR!", MB_ICONERROR | MB_OK);
		return;
	}
#endif //DEBUG
	WNDCLASSEX  wndclass = {
		sizeof(WNDCLASSEX),
		CS_ENABLE | CS_OWNDC | CS_HREDRAW,
		WndProc,
		0,
		0,
		sys_module,
		NULL,
		NULL,
		NULL,
		NULL,
		TG_MAIN_WINDOW_HANDLE,
		NULL
	};

#ifdef DEBUG
	if (!
#endif // DEBUG
		RegisterClassEx(&wndclass)
#ifndef DEBUG
		;
#endif
#ifdef DEBUG
		) {
		MessageBox(NULL, L"Registering window class failed, sorry!", L"ERROR!", MB_ICONERROR | MB_OK);
		return;
	}
#endif // DEBUG

}

void getMonitor() {
}

void Window::destroy() {
#ifdef _WIN32
	DestroyWindow(this->__impl_window);
#endif 
	vkDestroySurfaceKHR(instance, this->surface, nullptr);
}

void Window::pollevents() {
    #ifdef _WIN32
	MSG msg;
	while (PeekMessage(&msg, this->__impl_window, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
    #endif
}