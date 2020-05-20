#pragma once

#include "../util/VectorUtil.hpp"

#include <glm/glm.hpp>

namespace tge::io {
	/*
	 * These variables can be used to check if the buttons are down;
	 */
	extern bool FIRST_MOUSE_BUTTON;
	extern bool SECOND_MOUSE_BUTTON;
	extern bool THIRED_MOUSE_BUTTON;

	/*
	 * Internal mouse handling list
	 */
	extern std::vector<void(*)(glm::vec2, glm::vec2)> mouse_handler;

	/*
	 * Internal mouse handling method
	 */
	void inputupdate(glm::vec2 pos, glm::vec2 delta);

	/*
	 * With this method you can add a Listener that is called everytime an input is detected
	 */
	void addListener(void(*mouse)(glm::vec2, glm::vec2));
}