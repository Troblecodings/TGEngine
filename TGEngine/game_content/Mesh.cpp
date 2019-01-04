#include "Mesh.hpp"

void Mesh::consume(VertexBuffer * vrt, IndexBuffer * ind) {
	uint32_t sz = vrt->count_of_points;
	vrt->addAll(this->vertices.data(), this->vertices.size());
	for(uint32_t nt : this->indices)
	{
		ind->addIndex(sz + nt);
	}
}

void loadFromFBX(Mesh* mesh, char* path) {
	uint32_t ind = load(path);
	fbxsdk::FbxMesh* msh = FBX_Dictionary::fbx_meshes[ind];
	glm::vec4 color = FBX_Dictionary::colors[ind];
	Texture* tex = &FBX_Dictionary::textures[ind];

	fbxsdk::FbxVector4* arr = msh->GetControlPoints();
	FbxStringList lUVNames;
	msh->GetUVSetNames(lUVNames);

	FbxVector2 uv;
	bool bol;
	int index;
	for (int j = 0; j < msh->GetPolygonCount(); j++) {
		for (int i = 0; i < msh->GetPolygonSize(j); i++) {
			index = msh->GetPolygonVertex(j, i);
			msh->GetPolygonVertexUV(j, i, lUVNames[0], uv, bol);
			double* ptr = (double*)arr[index];
			TGVertex vertx = {
				{ ptr[0], ptr[1], ptr[2]},
				color,
				{ uv[0], 1 - uv[1] },
				tex->index
			};
			bol = false;
			for (size_t b = 0; b < mesh->vertices.size(); b++)
			{
				if (mesh->vertices[b] == vertx) {
					TG_VECTOR_APPEND_NORMAL(mesh->indices, (uint32_t)b)
					bol = true;
					break;
				}
			}
			if (!bol) {
				TG_VECTOR_APPEND_NORMAL(mesh->vertices, vertx)
				uint32_t idx = (uint32_t)last_size;
				TG_VECTOR_APPEND_NORMAL(mesh->indices, idx)
			}
		}
	}
}
