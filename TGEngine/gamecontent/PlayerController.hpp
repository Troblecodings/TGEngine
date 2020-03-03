#pragma once

#include "../Stdbase.hpp"

namespace tge {
	namespace gmc {
		// TODO make this modulare
		// Perhaps autogenerate

		/* Holds the input comping from the window,
		 * used to pass the input values platform 
		 * from the window to the player controller independently
		 */
		struct Input {
			float deltax;
			float deltay;
			float x;
			float y;
		};

		/* Function pointer to handle player inputs
		 */
		extern void(*playercontroller)(Input);
	}
}
