#pragma once

#include "stdbase.hpp"
#include "Pipeline\Buffer\VertexBuffer.hpp"
#include "fbxsdk.h"

SINCE(0, 0, 3)
fbxsdk::FbxNode* load(char* name);

SINCE(0, 0, 3)
void addToDraw(fbxsdk::FbxNode* node, VertexBuffer* buffer);