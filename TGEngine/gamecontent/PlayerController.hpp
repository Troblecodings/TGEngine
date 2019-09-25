#pragma once

#include "../Stdbase.hpp"

namespace tge {
	namespace gmc {
		// TODO make this modulare
		// Perhaps autogenerate

		struct Input {
			float x1;
			float y1;
			float x2;
			float y2;
		};

		extern void(*playercontroller)(Input*);
	}
}