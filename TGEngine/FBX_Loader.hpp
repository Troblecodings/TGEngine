#pragma once

#include "stdbase.hpp"
#include "Pipeline\Buffer\VertexBuffer.hpp"
#include <fbxsdk.h>

SINCE(0, 0, 3)
void load(char* name, FbxMesh* buffer);

SINCE(0, 0, 3)
void addToDraw(FbxMesh* mesh, VertexBuffer* buffer);