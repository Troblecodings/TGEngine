#pragma once

#include <vector>
#include <functional>

namespace tge::io {

	/*
	 * Internal keayboard handling list
	 */
	extern std::vector<std::function<void(uint16_t, bool)>> keyboardHandler;

	/*
	 * Internal keyinput handling method
	 */
	void implKeyUpdate(uint16_t key, bool state);

}
