#pragma once

#include "../Stdbase.hpp"
#include "../pipeline/buffer/UniformBuffer.hpp"

extern UniformBuffer light_buffer;

void initLight();

void setLightPosition(glm::vec3 pos);