#pragma once

#include "BaseDrawLib.hpp"

SINCE(0, 0, 2)
void drawRectangle(TGVertex vert, float xsize, float ysize, VertexBuffer* buffer, IndexBuffer* ibuffer);

SINCE(0, 0, 3)
void drawRectangleZ(TGVertex vert, float xsize, float depth, VertexBuffer* buffer, IndexBuffer* ibuffer);