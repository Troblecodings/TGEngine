#pragma once

#include "../Stdbase.hpp"
#include "../gamecontent/Material.hpp"
#include "../gamecontent/Actor.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"

#define TGR_VERSION_1 1

struct Map {
	std::vector<tge::tex::TextureInputInfo> textures;
	std::vector<tge::gmc::Material> materials;
	std::vector<tge::gmc::ActorInputInfo> actors;
};

void loadResourceFile(const char* name, Map* map);