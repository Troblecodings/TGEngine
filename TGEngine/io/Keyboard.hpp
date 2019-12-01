#pragma once

#include "../util/VectorUtil.hpp"
#include "../util/Annotations.hpp"

namespace tg_io {

	/*
	 * Internal keayboard handling list
	 */
	extern std::vector<void(*)(uint16_t, bool)> keyboard_handler;

	/*
	 * Internal keyinput handling method
	 */
	void __impl_key_update(uint16_t key, bool state);

	/*
	 * A Listener, which is called every time a key input is detected, can be added with this method
	 */
	void addKeyListener(void(*key_handle)(uint16_t, bool));

}
