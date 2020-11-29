#include "../../public/gamecontent/PlayerController.hpp"

namespace tge {
	namespace gmc {

		void(*playercontroller)(Input) = [](auto in) {};

	}
}