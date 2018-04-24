#pragma once

#include "BaseDrawLib.hpp"

SINCE(0, 0, 2)
void drawRectangle(Vertex vert, float xsize, float ysize, VertexBuffer* vertices);

SINCE(0, 0, 2)
void drawRectangleWithTexture(Vertex vert, float xsize, float ysize, VertexBuffer* buffer);

SINCE(0, 0, 3)
void drawRectangleZ(Vertex vert, float xsize, float depth, VertexBuffer* buffer);