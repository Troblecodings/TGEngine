#pragma once

#include "../Stdbase.hpp"
#include "../gamecontent/Material.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"

#define TGR_VERSION_1 1

struct Map {
	tge::tex::Texture* textures;
	tge::gmc::Material* materials;
};

Map loadResourceFile(char* name);