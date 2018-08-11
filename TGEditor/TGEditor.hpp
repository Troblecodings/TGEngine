#pragma once

#define DEBUG
#include <stdbase.hpp>

#include "FBX_Loader.hpp"
#include <TGEngine.hpp>
#include <Drawlib\DrawLib.hpp>
#include <IO\LoadFont.hpp>
#include <Pipeline\CommandBuffer.hpp>
#include "IO\ImagePNG.hpp"
#include <TGVertex.hpp>

extern Texture tex1;
extern Texture tex2;
extern fbxsdk::FbxMesh* mesh;
extern Font arial;

struct Editor : App
{
	Editor() : App() {};

	virtual void drawloop(VertexBuffer* verticies);
};

void drawGrid(VertexBuffer* buffer);

void drawLineH(TGVertex start, float length, VertexBuffer* buffer);

void drawLineV(TGVertex start, float length, VertexBuffer* buffer);

int main();