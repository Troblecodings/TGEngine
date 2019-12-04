#pragma once

#include "../../Stdbase.hpp"
#include "Camera.hpp"

namespace tge {
	namespace gmc {

		struct TopDownCamera {
			float positionx;
			float positiony;
			uint32_t screenwidth;
			uint32_t screenheight;
		};

		void setTopDownCamera(TopDownCamera* camera);

	}
}

