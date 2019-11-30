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
	 * With this method you can add a Listener that is called everytime a keyinput is detected
	 */
	void addKeyListener(void(*key_handle)(uint16_t, bool));

}