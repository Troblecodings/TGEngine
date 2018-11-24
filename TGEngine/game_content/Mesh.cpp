#include "Mesh.hpp"

void Mesh::consume(VertexBuffer * vrt, IndexBuffer * ind) {
	size_t sz = vrt->count_of_points;
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
	Texture tex = FBX_Dictionary::textures[ind];
	size_t siz = msh->GetControlPointsCount();
	fbxsdk::FbxVector4* arr = msh->GetControlPoints();
	for (size_t i = 0; i < siz; i++)
	{
		TGVertex vert;
		vert.position.x = (double)arr->mData[0];
		vert.position.y = (double)arr->mData[1];
		vert.position.z = (double)arr->mData[2];

		TG_VECTOR_APPEND(mesh->vertices, &vert)
	}
}
