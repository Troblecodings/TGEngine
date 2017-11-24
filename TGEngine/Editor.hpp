#pragma once

#include "TGEngine.hpp"
#include "Vertex.hpp"
#include "DrawLib.hpp"

struct EditorApp : App{

	virtual void drawloop(std::vector<Vertex>* verticies);

};