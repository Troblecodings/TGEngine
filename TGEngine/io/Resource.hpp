#pragma once

#include "../Stdbase.hpp"
#include "../gamecontent/Material.hpp"
#include "../gamecontent/Actor.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"

#define TGR_VERSION_1 1

struct Map {
    tge::tex::Sampler sampler;
	std::vector<tge::tex::Texture> textures;
};

/*
 * This loads a map from a resource file
 * this needs to be a tgr file:
 * https://troblecodings.com/fileformat.html
 * it automatically creates all ressources
 */
void loadResourceFile(const char* name, Map* map);
