#include "Mouse.hpp"

std::vector<Mouse*> mouse_handler;

void inputupdate(glm::vec2 pos) {
	for each(Mouse* ptr in mouse_handler) {
		ptr->mouse_move(pos);
	}
}

void addListener(Mouse* mouse) {
	uint32_t size = mouse_handler.size();
	mouse_handler.resize(size + 1);
	mouse_handler[size] = mouse;
}