#pragma once

#include "BaseDrawLib.hpp"

SINCE(0, 0, 2)
void drawRectangle(glm::vec3 pos, glm::vec4 color, uint32_t color_index, float xsize, float ysize, VertexBuffer* buffer, IndexBuffer* ibuffer);

SINCE(0, 0, 3)
void drawRectangleZ(TGVertex vert, float xsize, float depth, VertexBuffer* buffer, IndexBuffer* ibuffer);