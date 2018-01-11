#pragma once

#undef DEBUG
#define DEBUG
#include <TGEngine.hpp>
#include <DrawLib.hpp>
#include <LoadFont.hpp>

struct Editor : App
{
	virtual void drawloop(VertexBuffer* verticies);
};