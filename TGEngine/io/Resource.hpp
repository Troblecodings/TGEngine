#pragma once

#include "../Stdbase.hpp"

#define TGR_VERSION_1 1

struct Map {
	Texture* textures;
	Material* materials;
};

Map loadResourceFile(char* name);