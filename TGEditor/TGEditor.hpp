#pragma once

#undef DEBUG
#define DEBUG
#include <TGEngine.hpp>
#include <Drawlib\DrawLib.hpp>
#include <IO\LoadFont.hpp>
#include <Pipeline\CommandBuffer.hpp>
#include "IO\ImagePNG.hpp"

struct Editor : App
{
	virtual void drawloop(VertexBuffer* verticies);
};