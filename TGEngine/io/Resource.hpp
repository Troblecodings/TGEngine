#pragma once

#include "../Stdbase.hpp"
#include "../gamecontent/Material.hpp"
#include "../gamecontent/Actor.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"
#include "../pipeline/buffer/Memory.hpp"

namespace tge::io {

    constexpr uint32_t TGR_VERSION_1 = 1;
    constexpr uint32_t TGR_VERSION_2 = 2;

    constexpr uint32_t TGR_LATEST = TGR_VERSION_2;

    constexpr uint32_t TGE_MAP_BUFFER_COUNT = 3;

    struct Map {
        tge::tex::Sampler sampler;
        std::vector<tge::tex::Texture> textures;
        tge::buf::BufferObject mapBuffers[TGE_MAP_BUFFER_COUNT];
        std::vector<glm::vec4> transforms;
    };

    extern Map currentMap;

    /*
     * This loads a map from a resource file
     * this needs to be a tgr file:
     * https://troblecodings.com/fileformat.html
     * it automatically creates all ressources
     */
    void loadResourceFile(const char* name) noexcept;

    /*
     * This will destroy all vulkan tide ressources of the map
     */
    void destroyResource() noexcept;

}
