#include "Keyboard.hpp"

namespace tg_io {

	std::vector<void(*)(uint16_t, bool)> keyboard_handler;

	void __impl_key_update(uint16_t key, bool state)
	{
		for each (void(*key_handle)(uint16_t, bool) in keyboard_handler)
		{
			key_handle(key, state);
		}
	}

	void addKeyListener(void(*key_handle)(uint16_t, bool))
	{
		keyboard_handler.push_back(key_handle);
	}
}
