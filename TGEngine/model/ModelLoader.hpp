#pragma once

#include "../Stdbase.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"
#include "../gamecontent/Actor.hpp"

namespace tge {
	namespace mdl {

		void loadGltf(char* name, gmc::Model* model);
	}
}