#include "..\Stdbase.hpp"

extern std::vector<std::vector<char>> shader_data;
extern VkShaderStageFlagBits shader_flags[];

#define TG_FRAGMENT_SHADER_COLOR_ONLY_INDEX 0
#define TG_FRAGMENT_SHADER_TEXTURED_INDEX 1
#define TG_VERTEX_SHADER_COLOR_ONLY_INDEX 2
#define TG_VERTEX_SHADER_TEXTURED_INDEX 3