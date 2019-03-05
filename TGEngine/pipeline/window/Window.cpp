#include "Window.hpp"

uint32_t d_width = 0, d_height = 0;
std::vector<Window*> window_list;
HMODULE sys_module;

#if defined(_WIN32) || defined(_WIN64)
std::vector<HWND> __impl_window_list;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	size_t i;
	for (i = 0; i < __impl_window_list.size(); i++)
	{
		if (hwnd == __impl_window_list[i]) {
			break;
		}
	}
	Window* a_window = window_list[i];

	switch (msg)
	{
	case WM_QUIT:
	case WM_CLOSE:
	case WM_DESTROY:
		a_window->close_request = true;
		break;
	case WM_MOUSELEAVE:
	case WM_NCMOUSELEAVE:
		SetCursor(a_window->__impl_cursor);
		break;
	case WM_INPUT:
		uint32_t size;
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));
		RAWINPUT input;
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &input, &size, sizeof(RAWINPUTHEADER));
		if (a_window->consume_input) {
			SetCursorPos(a_window->middleX, a_window->middleY);
		}
		switch (input.header.dwType)
		{
		case RIM_TYPEMOUSE:
			tg_io::inputupdate({}, { input.data.mouse.lLastX, input.data.mouse.lLastY });
			break;
		case RIM_TYPEKEYBOARD:
			tg_io::__impl_key_update(input.data.keyboard.VKey, input.data.keyboard.Flags);
			break;
		default:
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		tg_io::FIRST_MOUSE_BUTTON = true;
		break;
	case WM_LBUTTONUP:
		tg_io::FIRST_MOUSE_BUTTON = false;
		break;
	case WM_MBUTTONDOWN:
		tg_io::THIRED_MOUSE_BUTTON = true;
		break;
	case WM_MBUTTONUP:
		tg_io::THIRED_MOUSE_BUTTON = false;
		break;
	case WM_RBUTTONDOWN:
		tg_io::SECOND_MOUSE_BUTTON = true;
		break;
	case WM_RBUTTONUP:
		tg_io::SECOND_MOUSE_BUTTON = false;
		break;
	case WM_KILLFOCUS:
		a_window->focused = false;
		break;
	case WM_SETFOCUS:
		a_window->focused = true;
		break;
	case WM_SYSCOMMAND:
		if (wParam == SC_MINIMIZE) {
			a_window->minimized = true;
		}
		else if (wParam == SC_RESTORE) {
			a_window->minimized = false;
		}
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
#endif

void createWindow(Window* window) {
	// IMPL getMonitor();

	bool fullscreen = properties->getBoolean("fullscreen");
	window->decorated = fullscreen ? false : properties->getBooleanOrDefault("decorated", true);
	window->consume_input = !(window->cursor = properties->getBooleanOrDefault("cursor", true));

	if (fullscreen) {
		GET_SIZE(d_width, d_height);
		window->width = d_width;
		window->height = d_height;
		window->x = d_width / 2 - window->width / 2;
		window->y = d_height / 2 - window->height / 2;
	}
	else if (properties->getBooleanOrDefault("center", true)) {
		GET_SIZE(d_width, d_height);
		window->height = properties->getIntOrDefault("height", 400);
		window->width = properties->getIntOrDefault("width", 400);
		window->x = d_width / 2 - window->width / 2;
		window->y = d_height / 2 - window->height / 2;
	}
	else {
		window->height = properties->getIntOrDefault("height", 400);
		window->width = properties->getIntOrDefault("width", 400);
		window->x = properties->getInt("posx");
		window->y = properties->getInt("posy");
	}

    #ifdef _WIN32 //Windows window creation

#ifdef DEBUG 
	uint32_t size;
	GetRawInputDeviceList(NULL, &size, sizeof(RAWINPUTDEVICELIST));
	RAWINPUTDEVICELIST* list = new RAWINPUTDEVICELIST[size];
	GetRawInputDeviceList(list, &size, sizeof(RAWINPUTDEVICELIST));
	for (size_t i = 0; i < size; i++)
	{
		uint32_t uint = 0;
		GetRawInputDeviceInfo(list[i].hDevice, RIDI_DEVICENAME, NULL, &uint);
		wchar_t* name = new wchar_t[uint + 1];
		GetRawInputDeviceInfo(list[i].hDevice, RIDI_DEVICENAME, name, &uint);
		name[uint] = '\0';
		OUT_LV_DEBUG("Name: " << name)
		RID_DEVICE_INFO info;
		uint = info.cbSize = sizeof(RID_DEVICE_INFO);
		GetRawInputDeviceInfo(list[i].hDevice, RIDI_DEVICEINFO, &info, &uint);
		switch (info.dwType)
		{
		case RIM_TYPEMOUSE:
			OUT_LV_DEBUG("Found mouse " << list[i].hDevice)
			OUT_LV_DEBUG("   ID: " << info.mouse.dwId)
			OUT_LV_DEBUG("   Number of buttons: " << info.mouse.dwNumberOfButtons)
			OUT_LV_DEBUG("   Sample rate: " << info.mouse.dwSampleRate)
			OUT_LV_DEBUG("   Has horizontal wheel: " << info.mouse.fHasHorizontalWheel)
			break;
		case RIM_TYPEKEYBOARD:
			OUT_LV_DEBUG("Found keyboard " << list[i].hDevice)
			OUT_LV_DEBUG("   Type: " << info.keyboard.dwType)
			OUT_LV_DEBUG("   Sub type: " << info.keyboard.dwSubType)
			OUT_LV_DEBUG("   Mode: " << info.keyboard.dwKeyboardMode)
			OUT_LV_DEBUG("   Num function keys: " << info.keyboard.dwNumberOfFunctionKeys)
			OUT_LV_DEBUG("   Num indicators: " << info.keyboard.dwNumberOfIndicators)
			OUT_LV_DEBUG("   Num keys total: " << info.keyboard.dwNumberOfKeysTotal)
			break;
		case RIM_TYPEHID:
			OUT_LV_DEBUG("Found hid " << list[i].hDevice)
			OUT_LV_DEBUG("   Product ID: " << info.hid.dwProductId)
			OUT_LV_DEBUG("   Vendor ID: " << info.hid.dwVendorId)
			OUT_LV_DEBUG("   Version Num: " << info.hid.dwVersionNumber)
			OUT_LV_DEBUG("   Usage: " << info.hid.usUsage)
			OUT_LV_DEBUG("   Usage Page: " << info.hid.usUsagePage)
			break;
		}
	}
#endif

	window_list.push_back(window);

	if (window->decorated) {
		//Char unicode conversation
		const char* ch = properties->getStringOrDefault("app_name", "TGEngine");
		size_t conv = strlen(ch) + 1;
		wchar_t* wc = new wchar_t[conv];
		mbstowcs_s(&conv, wc, conv, ch, _TRUNCATE);
		wc[conv] = '\0';

		DWORD style = WS_CLIPSIBLINGS | WS_CAPTION;
		if (!window->consume_input) {
			style |= WS_SYSMENU;
		}
		if (properties->getBooleanOrDefault("minimizeable", true)) {
			style |= WS_MINIMIZEBOX;
		}
		if (properties->getBooleanOrDefault("maximizable", true)) {
			style |= WS_MAXIMIZEBOX;
		}
		if (properties->getBooleanOrDefault("resizeable", true)) {
			style |= WS_SIZEBOX;
		}
		window->__impl_window = CreateWindowEx(WS_EX_APPWINDOW, TG_MAIN_WINDOW_HANDLE, (LPCWCHAR)wc, style, window->x, window->y, window->width + 16, window->height + 39, nullptr, nullptr, sys_module, nullptr);
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
	__impl_window_list.push_back(window->__impl_window);

	ShowWindow(window->__impl_window, SW_SHOW);
	UpdateWindow(window->__impl_window);

	if (window->consume_input) {
		LPRECT win_rect = new RECT();
		GetWindowRect(window->__impl_window, win_rect);
		window->middleX = (long)(win_rect->left + win_rect->right / 2.0f);
		window->middleY = (long)(win_rect->top + win_rect->bottom / 2.0f);
		SetCursorPos(window->middleX, window->middleY);
	}

	window->__impl_cursor = window->cursor ? LoadCursor(nullptr, IDC_ARROW) : NULL;
	SetCursor(window->__impl_cursor);

	RAWINPUTDEVICE Rid[2];
	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x02;
	if (window->consume_input) {
		Rid[0].dwFlags = RIDEV_NOLEGACY | RIDEV_CAPTUREMOUSE;   // adds HID mouse and also ignores legacy mouse messages if it consumes input
	}
	else {
		Rid[0].dwFlags = 0;
	}
	Rid[0].hwndTarget = window->__impl_window;

	Rid[1].usUsagePage = 0x01;
	Rid[1].usUsage = 0x06;
	Rid[1].dwFlags = 0;   // adds HID keyboard
	Rid[1].hwndTarget = window->__impl_window;
	RegisterRawInputDevices(Rid, 2, sizeof(Rid[0]));
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

void destroyWindows() {
	for each (Window* var in window_list)
	{
#if defined(_WIN32) || defined(_WIN64)
		DestroyWindow(var->__impl_window);
#endif 
		vkDestroySurfaceKHR(instance, var->surface, nullptr);
	}
}

void createWindowSurfaces() {
	for each (Window* var in window_list)
	{
#ifdef _WIN32 
		VkWin32SurfaceCreateInfoKHR surface_create_info = {
			VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			sys_module,
			var->__impl_window
		};
		last_result = vkCreateWin32SurfaceKHR(instance, &surface_create_info, nullptr, &var->surface);
		HANDEL(last_result)
#endif 
	}
}

void Window::pollevents() {
	#if defined(_WIN32) || defined(_WIN64)
	MSG msg;
	while (PeekMessage(&msg, this->__impl_window, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
    #endif
}