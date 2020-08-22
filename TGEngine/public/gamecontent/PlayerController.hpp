#pragma once

#include "../../public/Stdbase.hpp"

namespace tge {
	namespace gmc {
		// TODO make this modulare
		// Perhaps autogenerate

		/* Holds the input comping from the window,
		 * used to pass the input values platform 
		 * from the window to the player controller independently
		 */
		struct Input {
			float inputX;
			float inputY;
		};

		/* Function pointer to handle player inputs
		 */
		extern void(*playercontroller)(Input);
	}
}
