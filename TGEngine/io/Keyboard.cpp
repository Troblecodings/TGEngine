#include "Keyboard.hpp"

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
	TG_VECTOR_APPEND_NORMAL(keyboard_handler, key_handle)
}
