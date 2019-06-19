#pragma once

#include <Stdbase.hpp>

#include <model/FBX_Loader.hpp>
#include <TGEngine.hpp>
#include <io/Font.hpp>
#include <pipeline/CommandBuffer.hpp>
#include <util/TGVertex.hpp>
#include <gamecontent/Mesh.hpp>
#include <gamecontent/Material.hpp>
#include <gamecontent/FirstPersonCamera.hpp>
#include <gamecontent/StaticCamera.hpp>
#include <ui/UITextureComponent.hpp>
#include <ui/UITextComponent.hpp>

using namespace tge::ui;

extern Mesh mesh;
extern Mesh mesh2;
extern Camera camera;
extern UIEntity entity;
extern Mesh map;

struct Editor : App
{
	Editor() : App() {};
};

void init();

void drawloop(IndexBuffer* ibuffer, VertexBuffer* verticies);

int main();