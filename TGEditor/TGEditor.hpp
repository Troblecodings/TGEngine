#pragma once

#include <Stdbase.hpp>

#include <model/FBX_Loader.hpp>
#include <TGEngine.hpp>
#include <drawlib/DrawLib.hpp>
#include <io/LoadFont.hpp>
#include <pipeline/CommandBuffer.hpp>
#include <util/TGVertex.hpp>
#include "game_content/Mesh.hpp"
#include "game_content/FirstPersonCamera.hpp"
#include "game_content/StaticCamera.hpp"
#include "ui/UIButton.hpp"
#include "ui/UITexture.hpp"

extern Texture tex1;
extern Texture tex2;
extern Mesh mesh;
extern Mesh mesh2;
extern Font arial;
extern Camera camera;
extern UIButton* btn;
extern UITexture* tex;

struct Editor : App
{
	Editor() : App() {};
};

void init();

void drawloop(IndexBuffer* ibuffer, VertexBuffer* verticies);

void drawGrid(VertexBuffer* buffer, IndexBuffer* vbuffer);

void drawLineH(TGVertex start, float length, VertexBuffer* buffer, IndexBuffer* vbuffer);

void drawLineV(TGVertex start, float length, VertexBuffer* buffer, IndexBuffer* vbuffer);

int main();