#pragma once

#include <Stdbase.hpp>

#include <model/FBX_Loader.hpp>
#include <TGEngine.hpp>
#include <pipeline/CommandBuffer.hpp>
#include <util/TGVertex.hpp>
#include <gamecontent/Mesh.hpp>
#include <gamecontent/Material.hpp>
#include <gamecontent/FirstPersonCamera.hpp>
#include <gamecontent/StaticCamera.hpp>
#include <ui/UITextureComponent.hpp>
#include <ui/UITextComponent.hpp>

using namespace tge::ui;

extern tge::gmc::Mesh mesh;
extern tge::gmc::Mesh mesh2;
extern tge::gmc::Camera camera;
extern UIEntity entity;
extern tge::gmc::Mesh map;

struct Editor : App {
	Editor() : App() {};
};

void init();

void drawloop(IndexBuffer* ibuffer, VertexBuffer* verticies);
