#pragma once

#include "Stdbase.hpp"
#include "App.hpp"
#include "game_content\FirstPersonCamera.hpp"
#include "pipeline\Pipeline.hpp"
#include "pipeline\Draw.hpp"

SINCE(0, 0, 1)
void initTGEngine(Window* window, void(*draw)(IndexBuffer*, VertexBuffer*), void(*init)(void));