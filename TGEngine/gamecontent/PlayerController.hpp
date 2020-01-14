#pragma once

#include "../Stdbase.hpp"

namespace tge {
	namespace gmc {
		// TODO make this modulare
		// Perhaps autogenerate

		/* This holds the Input comping from the window
		 * it is used to pass the input values platform 
		 * independent from the window to the playercontroller
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
