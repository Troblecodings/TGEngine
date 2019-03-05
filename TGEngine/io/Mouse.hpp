#pragma once

#include "../util/VectorUtil.hpp"
#include "../util/Annotations.hpp"
#include <glm/glm.hpp>

namespace tg_io {
	/*
	 * These variables can be used to track the mouses position
	 */
	extern glm::vec2 pos;
	extern glm::vec2 delta;

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
	SINCE(0, 0, 3)
	void inputupdate(glm::vec2 pos, glm::vec2 delta);

	/*
	 * With this method you can add a Listener that is called everytime an input is detected
	 */
	SINCE(0, 0, 3)
	void addListener(void(*mouse)(glm::vec2, glm::vec2));
}