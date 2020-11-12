#pragma once

#include "../../public/Stdbase.hpp"
#include "../../public/gamecontent/Material.hpp"
#include "../../public/gamecontent/Actor.hpp"
#include "../../public/pipeline/buffer/Texturebuffer.hpp"
#include "../../public/pipeline/buffer/Memory.hpp"

namespace tge::io {

    constexpr uint32_t TGR_VERSION_1 = 1;
    constexpr uint32_t TGR_VERSION_2 = 2;
    constexpr uint32_t TGR_VERSION_3 = 3;
    constexpr uint32_t TGR_LATEST = TGR_VERSION_3;

    constexpr uint32_t TGE_MAP_BUFFER_COUNT = 4;
    constexpr uint32_t INSTANCE_LENGTH = 4 * 6;

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
     * This will destroy all vulkan ressources of the map
     */
    void destroyResource() noexcept;

}
