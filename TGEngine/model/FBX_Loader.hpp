#pragma once

#include "Stdbase.hpp"
#include "..\Pipeline\Buffer\VertexBuffer.hpp"
#include "fbxsdk.h"
#include "..\Pipeline\Buffer\Texturebuffer.hpp"

#ifdef DEBUG
#define FBX_CHECK(sucess)\
if (!sucess) {\
	OUT_LV_DEBUG("Failed to load " << name)\
	OUT_LV_DEBUG("Error: " << importer->GetStatus().GetErrorString())\
    return nullptr;\
}
#else
sucess;
#endif

namespace FBX_Dictionary {
	extern std::vector<fbxsdk::FbxMesh*> fbx_meshes;
	extern std::vector<char*> names;
	extern std::vector<glm::vec4> colors;
	extern std::vector<Texture> textures;

	SINCE(0, 0, 3)
	fbxsdk::FbxMesh* addAll(fbxsdk::FbxNode* mesh);
	
	SINCE(0, 0, 3)
	void addToDraw(VertexBuffer* buffer);
};

SINCE(0, 0, 3)
fbxsdk::FbxMesh* load(char* name);

