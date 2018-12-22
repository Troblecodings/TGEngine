#include "Mesh.hpp"

void Mesh::consume(VertexBuffer * vrt, IndexBuffer * ind) {
	size_t sz = ind->index_count;
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
	size_t siz = msh->GetControlPointsCount();
	fbxsdk::FbxVector4* arr = msh->GetControlPoints();
	for (size_t i = 0; i < siz; i++)
	{
		TGVertex vert;
		vert.position.x = (double)arr[i].mData[0];
		vert.position.y = (double)arr[i].mData[1];
		vert.position.z = (double)arr[i].mData[2];
		vert.color = color;
		vert.color_only = tex->index;
		fbxsdk::FbxLayerElementArrayTemplate<FbxVector2>* pLockableArray;
		if (msh->GetTextureUV(&pLockableArray)) {
			FbxVector2 vector = pLockableArray->GetAt(i);
			vert.uv = { vector.mData[0], 1 - vector.mData[1] };
		}

		TG_VECTOR_APPEND(mesh->vertices, &vert)
	}
	for (size_t j = 0; j < msh->GetPolygonCount(); j++) {
		for (size_t i = 0; i < msh->GetPolygonSize(j); i++) {
			int index = msh->GetPolygonVertex(j, i);
			TG_VECTOR_APPEND(mesh->indices, &index)
		}
	}
}
