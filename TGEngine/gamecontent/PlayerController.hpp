#pragma once

#include "../Stdbase.hpp"

namespace tge {
	namespace gmc {
		// TODO make this modulare
		// Perhaps autogenerate

		/* Holds the input comping from the window,
		 * it's used to pass the input values platform 
		 * from the window to the player controller independently
		 */
		struct Input {
			float x1;
			float y1;
			float x2;
			float y2;
		};

		/* This is a function pointer to handle player inputs
		 */
		extern void(*playercontroller)(Input*);
	}
}
