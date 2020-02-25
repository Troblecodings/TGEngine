#pragma once

#include "../../Stdbase.hpp"
#include "Camera.hpp"

namespace tge {
	namespace gmc {

		/* 
		 * Holds the information needed to supply a top-down camera
		 */
		struct TopDownCamera {
			float    positionx;
			float    positiony;
			uint32_t screenwidth;
			uint32_t screenheight;
		};

		/* 
		 * Loads the top-down matrix onto the device,
		 * used as basic transform
		 */
		void setTopDownCamera(TopDownCamera* camera);

	}
}
