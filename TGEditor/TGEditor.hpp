#pragma once

#include <Stdbase.hpp>

#include <model\FBX_Loader.hpp>
#include <TGEngine.hpp>
#include <drawlib\DrawLib.hpp>
#include <io\LoadFont.hpp>
#include <pipeline\CommandBuffer.hpp>
#include <util\TGVertex.hpp>

extern Texture tex1;
extern Texture tex2;
extern uint32_t mesh;
extern Font arial;

struct Editor : App
{
	Editor() : App() {};
};

void init();

void drawloop(IndexBuffer* ibuffer, VertexBuffer* verticies);

void drawGrid(VertexBuffer* buffer);

void drawLineH(TGVertex start, float length, VertexBuffer* buffer);

void drawLineV(TGVertex start, float length, VertexBuffer* buffer);

int main();