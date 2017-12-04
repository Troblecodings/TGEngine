#pragma once

#define DEBUG
#include "TGEngine.hpp"
#include "DrawLib.hpp"

struct Editor : App
{
	virtual void drawloop(std::vector<Vertex>* verticies);
};