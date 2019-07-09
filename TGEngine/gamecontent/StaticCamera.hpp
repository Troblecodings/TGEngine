#pragma once

#include "../Stdbase.hpp"
#include "Camera.hpp"

namespace tge {
	namespace gmc {

		void createStaticCamera(Camera* camera);

		void __impl_mouseinputHandleSC(Camera* camera, glm::vec2 pos, glm::vec2 delta);
	}
}