#pragma once

#include "../Stdbase.hpp"
#include "../pipeline/buffer/VertexBuffer.hpp"
#include <fbxsdk.h>
#include "../pipeline/buffer/Texturebuffer.hpp"

#ifdef DEBUG
#define FBX_CHECK(sucess)\
if (!sucess) {\
	OUT_LV_DEBUG("Failed to load " << name)\
	OUT_LV_DEBUG("Error: " << importer->GetStatus().GetErrorString())\
    return 0;\
}
#else
#define FBX_CHECK(sucess) sucess;
#endif

namespace FBX_Dictionary {
	extern std::vector<fbxsdk::FbxMesh*> fbx_meshes;
	extern std::vector<char*> names;
	extern std::vector<glm::vec4> colors;
	extern std::vector<Texture> textures;

	SINCE(0, 0, 3)
	uint32_t addAll(fbxsdk::FbxNode* mesh);
	
	SINCE(0, 0, 3)
	void addToDraw(VertexBuffer* buffer);
};

SINCE(0, 0, 3)
uint32_t load(char* name);

