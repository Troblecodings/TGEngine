#pragma once

#include "TGEngine.hpp"
#include "Vertex.hpp"

struct EditorApp : App{

	virtual void drawloop(std::vector<Vertex>* verticies);

};