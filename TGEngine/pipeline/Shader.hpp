#pragma once

#include <vector>
#include "../Stdbase.hpp"
#include "../io/Files.hpp"
#include "Device.hpp"
#include "../resources/ShaderData.hpp"

SINCE(0, 0, 1)
void createShaderInput(uint32_t location, uint32_t offset, VkFormat format);

SINCE(0, 0, 1)
void destroyShaders();