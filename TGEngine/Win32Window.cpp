#ifdef _WIN32

#include "Win32Window.hpp"

LPCWCHAR lresult;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_QUIT || msg == WM_CLOSE || msg == WM_DESTROY) {
		window.close_request = true;
		return 0;
	}
	else if (msg == WM_SYSCOMMAND && wParam == SC_MINIMIZE) {
		UNFINISHED

		// IMPL: minimize
		MessageBox(hwnd, L"Feature not usable in the current version!", L"Sorry!", MB_ICONINFORMATION | MB_OK);

		END_UNFINISHED
	}
	else {
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

void registerWindowClass() {
	
	WNDCLASSEX  wndclass = {
		sizeof(WNDCLASSEX),
		CS_ENABLE | CS_OWNDC | CS_HREDRAW,
		WndProc,
		0,
		0,
		GetModuleHandle(nullptr),
		NULL,
		NULL,
		NULL,
		NULL,
		WINDOW_HANDLE,
		NULL
	};

	if (!RegisterClassEx(&wndclass)) {
		MessageBox(NULL, L"Window creaton failed, sorry!", L"ERROR!", MB_ICONERROR | MB_OK);
		return;
	}
}

#endif
