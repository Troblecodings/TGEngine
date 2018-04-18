#include "Window.hpp"

uint32_t d_width = 0, d_height = 0;

#ifdef _WIN32
Window::Window(wchar_t* name) {
	this->__impl_cursor = this->cursor ? LoadCursor(nullptr, IDC_ARROW) : NULL;

	WNDCLASSEX  wndclass = {
		sizeof(WNDCLASSEX),
		CS_ENABLE | CS_OWNDC | CS_HREDRAW,
		this->WndProc,
		0,
		0,
		GetModuleHandle(nullptr),
		NULL,
		this->__impl_cursor,
		NULL,
		NULL,
		name,
		NULL
	};

	if (!RegisterClassEx(&wndclass)) {
		MessageBox(NULL, L"Window creaton failed, sorry!", L"ERROR!", MB_ICONERROR | MB_OK);
		return;
	}

	SetCursor(this->__impl_cursor);
	this->__impl_handle = name;
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_QUIT || msg == WM_CLOSE || msg == WM_DESTROY) {
		this->close_request = true;
		return NULL;
	}
	else if (msg == WM_SYSCOMMAND && wParam == SC_MINIMIZE) {
		UNFINISHED

		// IMPL: minimize
		MessageBox(hwnd, L"Feature not usable in the current version!", L"Sorry!", MB_ICONINFORMATION | MB_OK);

		END_UNFINISHED
	}
	else {
		if (msg == WM_MOUSELEAVE || msg == WM_NCMOUSELEAVE) {
			SetCursor(this->__impl_cursor);
		}
		else if (msg == WM_MOUSEMOVE) {
			inputupdate({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
			if (this->consume_input && this->focused) {
				LPRECT win_rect = new RECT();
				GetWindowRect(hwnd, win_rect);
				SetCursorPos((int)(win_rect->right - win_rect->left) / 2 + win_rect->left, (int)(win_rect->bottom - win_rect->top) / 2 + win_rect->top);
			}
		}
		else if (msg == WM_KILLFOCUS) {
			this->focused = false;
		}
		else if (msg == WM_SETFOCUS) {
			this->focused = true;
			if (this->consume_input) {
				LPRECT win_rect = new RECT();
				GetWindowRect(hwnd, win_rect);
				SetCursorPos((int)(win_rect->right - win_rect->left) / 2 + win_rect->left, (int)(win_rect->bottom - win_rect->top) / 2 + win_rect->top);
			}
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
#endif

void createWindow(Window window, nio::Properties* properties = nullptr) {
	// IMPL getMonitor();

	if (properties != nullptr) {
		bool fullscreen = properties->getBoolean("fullscreen").rvalue;
		window.decorated = fullscreen ? false : properties->getBoolean("decorated").rvalue;
		window.cursor = properties->getBoolean("cursor").rvalue;
		if (fullscreen) {
			GET_SIZE(d_width, d_height)
				width = d_width;
			height = d_height;
			x = d_width / 2 - width / 2;
			y = d_height / 2 - height / 2;
		}
		else if (properties->getBoolean("center").rvalue) {
			GET_SIZE(d_width, d_height)
			height = properties->getInt("height").rvalue;
			width = properties->getInt("width").rvalue;
			x = d_width / 2 - width / 2;
			y = d_height / 2 - height / 2;
		}
		else {
			height = properties->getInt("height").rvalue;
			width = properties->getInt("width").rvalue;
			x = properties->getInt("posx").rvalue;
			y = properties->getInt("posy").rvalue;
		}
	}
    #ifdef _WIN32 //Windows window createion
	if (window.decorated) {
		//Char unicode conversation
		if (properties != nullptr) {
			char* ch = properties->getString("app_name").value;
			const size_t cSize = strlen(ch) + 1;
			std::wstring wc(cSize, L'#');
			mbstowcs(&wc[0], ch, cSize);
		    window.__impl_window = CreateWindowEx(WS_EX_APPWINDOW, window.__impl_handle, (LPCWCHAR)wc.data(), WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, x, y, width + 16, height + 39, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
		}
		else {
			window.__impl_window = CreateWindowEx(WS_EX_APPWINDOW, window.__impl_handle, window.__impl_handle, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, x, y, width + 16, height + 39, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
		}
	}
	else {
	    window.__impl_window = CreateWindowEx(NULL, window.__impl_handle, nullptr, WS_POPUP | WS_VISIBLE | WS_SYSMENU, x, y, width, height, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
	}
	ShowWindow(window.__impl_window, SW_SHOW);
	UpdateWindow(window.__impl_window);
	if (window.consume_input) {
		LPRECT win_rect = new RECT();
		GetWindowRect(window.__impl_window, win_rect);
		SetCursorPos((int)(win_rect->right - win_rect->left) / 2 + win_rect->left, (int)(win_rect->bottom - win_rect->top) / 2 + win_rect->top);
	}
    #endif
}

void Window::createWindowSurface() {
    #ifdef _WIN32 
	VkWin32SurfaceCreateInfoKHR surface_create_info = {
		VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
	    nullptr,
	    0,
		GetModuleHandle(nullptr),
	    this->__impl_window
	};
	last_result = vkCreateWin32SurfaceKHR(instance, &surface_create_info, nullptr, &this->surface);
	HANDEL(last_result)
    #endif 
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