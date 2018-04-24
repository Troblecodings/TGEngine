#pragma once

#define DEBUG
#include <stdbase.hpp>

#include <TGEngine.hpp>
#include <Drawlib\DrawLib.hpp>
#include <IO\LoadFont.hpp>
#include <Pipeline\CommandBuffer.hpp>
#include "IO\ImagePNG.hpp"

struct Editor : App
{
	Editor() : App() {}

	virtual void drawloop(VertexBuffer* verticies);
};

void drawGrid(VertexBuffer* buffer);

void drawLineH(Vertex start, float length, VertexBuffer* buffer);

void drawLineV(Vertex start, float length, VertexBuffer* buffer);