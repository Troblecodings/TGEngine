#pragma once

#include "../Stdbase.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"
#include "../gamecontent/Mesh.hpp"

namespace tge {
	namespace mdl {

		void loadGltf(char* name, gmc::Mesh* mesh);
	}
}