#pragma once

#include "Annotations.hpp"
#include "VertexBuffer.hpp"

SINCE(0, 0, 2)
struct App {

	virtual void drawloop(VertexBuffer* buffer) = 0;

};