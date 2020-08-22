#include "../../public/io/Keyboard.hpp"

namespace tge::io {

	std::vector<std::function<void(uint16_t, bool)>> keyboardHandler;

	void implKeyUpdate(uint16_t key, bool state) {
		for (const auto& func : keyboardHandler)
			func(key, state);
	}
}
