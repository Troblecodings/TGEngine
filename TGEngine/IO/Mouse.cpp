#include "Mouse.hpp"

glm::vec2 last_pos;
std::vector<Mouse*> mouse_handler;

bool FIRST_MOUSE_BUTTON;
bool SECOND_MOUSE_BUTTON;
bool THIRED_MOUSE_BUTTON;

void inputupdate(glm::vec2 pos) {
	glm::vec2 delta_pos = pos - last_pos;
	last_pos = pos;
	for each(Mouse* ptr in mouse_handler) {
		ptr->mouse_move(pos, delta_pos);
	}
}

void addListener(Mouse* mouse) {
	uint32_t size = mouse_handler.size();
	mouse_handler.resize(size + 1);
	mouse_handler[size] = mouse;
}