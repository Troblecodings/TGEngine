#pragma once

#include "../Stdbase.hpp"
#include "../pipeline/buffer/VertexBuffer.hpp"
#include <fbxsdk.h>
#include "../pipeline/buffer/Texturebuffer.hpp"
#include "../Error.hpp"
#include "../gamecontent/Mesh.hpp"

#ifdef DEBUG
#define FBX_CHECK(sucess) ASSERT_NONE_NULL_DB(sucess, "Failed to load " << name << " with error " << importer->GetStatus().GetErrorString(), TG_ERR_DB_NULLPTR)
#else
#define FBX_CHECK(sucess) sucess;
#endif

namespace tg_model {

	SINCE(0, 0, 3)
		void load(char* name, tge::gmc::Mesh* mesh);
};
