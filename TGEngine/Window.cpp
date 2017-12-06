#include "Window.hpp"

uint32_t x = 0, y = 0, width = 0, height = 0, d_width = 0, d_height = 0;
Window window;
VkSurfaceKHR surface;

void createWindow(nio::Properties properties) {
	// IMPL getMonitor();

	// IMPL: properties.getBoolean("decorated").rvalue);
	bool fullscreen = properties.getBoolean("fullscreen").rvalue;
	if (fullscreen) {
		// IMPL DECORATED
		GET_SIZE(d_width, d_height)
		width = d_width;
		height = d_height;
		x = d_width / 2 - width / 2;
		y = d_height / 2 - height / 2;
	}
	else if (properties.getBoolean("center").rvalue) {
		GET_SIZE(d_width, d_height)
		height = properties.getInt("height").rvalue;
		width = properties.getInt("width").rvalue;
		x = d_width / 2 - width / 2;
		y = d_height / 2 - height / 2;
	}
	else {
		height = properties.getInt("height").rvalue;
		width = properties.getInt("width").rvalue;
		x = properties.getInt("posx").rvalue;
		y = properties.getInt("posy").rvalue;
	}
    #ifdef _WIN32
	registerWindowClass();
	char* ch = properties.getString("app_name").value;
	const size_t cSize = strlen(ch) + 1;
	std::wstring wc(cSize, L'#');
	mbstowcs(&wc[0], ch, cSize);
	window = { CreateWindowEx(WS_EX_APPWINDOW , WINDOW_HANDLE,  (LPCWCHAR)wc.data(), WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, x, y, width + 16, height + 39, nullptr, nullptr, GetModuleHandle(nullptr),nullptr) };
	ShowWindow(window.__impl_window, SW_SHOW);
	UpdateWindow(window.__impl_window);
    #endif
}

void createWindowSurface() {
    #ifdef _WIN32 
	VkWin32SurfaceCreateInfoKHR surface_create_info = {
		VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
	    nullptr,
	    0,
		GetModuleHandle(nullptr),
	    window.__impl_window
	};
	last_result = vkCreateWin32SurfaceKHR(instance, &surface_create_info, nullptr, &surface);
	HANDEL(last_result)
    #endif 
}

void getMonitor() {
}

void destroyWindow() {
#ifdef _WIN32
	DestroyWindow(window.__impl_window);
#endif 
	vkDestroySurfaceKHR(instance, surface, nullptr);
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