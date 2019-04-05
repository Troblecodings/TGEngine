#pragma once

#include "BaseDrawLib.hpp"

SINCE(0, 0, 2)
void drawRectangle(glm::vec3 pos, float xsize, float zsize, VertexBuffer* buffer, IndexBuffer* ibuffer);

SINCE(0, 0, 3)
void drawRectangleZ(glm::vec3 pos, float xsize, float ysize, VertexBuffer* buffer, IndexBuffer* ibuffer);