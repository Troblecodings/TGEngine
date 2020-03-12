#pragma once

#include "../Stdbase.hpp"
#include "../gamecontent/Material.hpp"
#include "../gamecontent/Actor.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"

namespace tge::io {

    constexpr uint32_t TGR_VERSION_1 = 1;
    constexpr uint32_t TGR_VERSION_2 = 2;

    constexpr uint32_t TGR_LATEST = TGR_VERSION_2;

    struct Map {
        tge::tex::Sampler sampler;
        tge::tex::Texture* textures;
    };

    /*
     * This loads a map from a resource file
     * this needs to be a tgr file:
     * https://troblecodings.com/fileformat.html
     * it automatically creates all ressources
     */
    void loadResourceFile(const char* name, Map* map);

}
