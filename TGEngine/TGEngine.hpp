#pragma once

#include "Stdbase.hpp"
#include "App.hpp"
#include "pipeline/Pipeline.hpp"
#include "pipeline/Draw.hpp"
#include "game_content/Actor.hpp"
#include "game_content/Light.hpp"
#include "ui/UIEntity.hpp"

SINCE(0, 0, 1)
void initTGEngine(Window* window, void(*draw)(IndexBuffer*, VertexBuffer*), void(*init)(void));