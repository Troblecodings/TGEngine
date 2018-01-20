#pragma once

#include "Util/Annotations.hpp"
#include "Pipeline/Buffer/VertexBuffer.hpp"

SINCE(0, 0, 2)
struct App {

	virtual void drawloop(VertexBuffer* buffer) = 0;

};