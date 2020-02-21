#pragma once

#include "../Stdbase.hpp"
#include "../gamecontent/Material.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"

#define TGR_VERSION_1 1

struct Map {
	std::vector<TextureInputInfo> textures;
	std::vector<Material> materials;
	std::vector<ActorInputInfo> actors;
};

void loadResourceFile(const char* name, Map* map);