#pragma once

#include "BaseDrawLib.hpp"

SINCE(0, 0, 2)
void drawRectangle(TGVertex vert, float xsize, float ysize, VertexBuffer* vertices);

SINCE(0, 0, 2)
void drawRectangleWithTexture(TGVertex vert, float xsize, float ysize, VertexBuffer* buffer);

SINCE(0, 0, 3)
void drawRectangleZ(TGVertex vert, float xsize, float depth, VertexBuffer* buffer);