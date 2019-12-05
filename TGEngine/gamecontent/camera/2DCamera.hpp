#pragma once

#include "../../Stdbase.hpp"
#include "Camera.hpp"

namespace tge {
	namespace gmc {

		/* This is a holder struct for 
		 */
		struct TopDownCamera {
			float positionx;
			float positiony;
			uint32_t screenwidth;
			uint32_t screenheight;
		};

		/* Loads the top down matrix onto the device
		 * this is used as basic tranform
		 * 
		 */
		void setTopDownCamera(TopDownCamera* camera);

	}
}

