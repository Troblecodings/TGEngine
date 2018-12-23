#include "Mesh.hpp"

void Mesh::consume(VertexBuffer * vrt, IndexBuffer * ind) {
	uint32_t sz = ind->index_count;
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
	int siz = msh->GetControlPointsCount();
	fbxsdk::FbxVector4* arr = msh->GetControlPoints();
	for (int i = 0; i < siz; i++)
	{
		TGVertex vert;
		double* ptr = (double*)arr[i];
		vert.position.x = (float)ptr[0];
		vert.position.y = (float)ptr[1];
		vert.position.z = (float)ptr[2];
		vert.color = color;
		vert.color_only = tex->index;
		fbxsdk::FbxLayerElementArrayTemplate<FbxVector2>* pLockableArray;
		if (msh->GetTextureUV(&pLockableArray)) {
			double* vector = (double*)pLockableArray->GetAt(i);
			vert.uv = { vector[0], 1 - vector[1] };
		}

		TG_VECTOR_APPEND(mesh->vertices, &vert)
	}
	for (int j = 0; j < msh->GetPolygonCount(); j++) {
		for (int i = 0; i < msh->GetPolygonSize(j); i++) {
			int index = msh->GetPolygonVertex(j, i);
			TG_VECTOR_APPEND(mesh->indices, &index)
		}
	}
}
