#pragma once

#include "../Stdbase.hpp"
#include "../pipeline/buffer/UniformBuffer.hpp"

extern UniformBuffer light_buffer; // the light buffer for the postion of the light

/*
 * Internal method to initialize the light buffer
 */
INTERNAL
SINCE(0, 0, 4)
void initLight();

/*
 * Change the possition of the diffuse light
 */
SINCE(0, 0, 4)
void setLightPosition(glm::vec3 pos);