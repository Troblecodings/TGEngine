#include "Mouse.hpp"

namespace tge::io {

	std::vector<void(*)(glm::vec2, glm::vec2)> mouse_handler;

	bool FIRST_MOUSE_BUTTON;
	bool SECOND_MOUSE_BUTTON;
	bool THIRED_MOUSE_BUTTON;

	void inputupdate(glm::vec2 pos, glm::vec2 delta) {
		for each (void(*ptr)(glm::vec2, glm::vec2) in mouse_handler) {
			ptr(pos, delta);
		}
	}

	void addListener(void(*mouse)(glm::vec2, glm::vec2)) {
		mouse_handler.push_back(mouse);
	}

}