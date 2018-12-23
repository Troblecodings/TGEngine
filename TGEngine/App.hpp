#pragma once

#include "util/Annotations.hpp"
#include "pipeline/buffer/VertexBuffer.hpp"

SINCE(0, 0, 2)
struct App {

	App() : main_window((wchar_t*)TG_MAIN_WINDOW_HANDLE) {
		size_t size = window_list.size();
		window_list.resize(size + 1);
		window_list[size] = &this->main_window;
	}

	Window main_window;

};