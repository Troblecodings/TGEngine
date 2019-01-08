#include "Mesh.hpp"

void Mesh::consume(VertexBuffer * vrt, IndexBuffer * ind) {
	uint32_t sz = (uint32_t)vrt->count_of_points;
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
	if (!tex->texture_path)tex->index = COLOR_ONLY;

	int index;

	/*
	 * Proccess data
	 */
	for (int j = 0; j < msh->GetPolygonCount(); j++) {
		int triangle_size = msh->GetPolygonSize(j);
		if (triangle_size >= 3) {
			triangle_size -= 2; // Because every convex pylogen without holes can be triangulated with n-2 triangles
		}
		else if (triangle_size < 3) {
			continue; // Don't support lines or dots
		}

		/*
         * Preproccess data for vertex buffer
		 * This is triangulated via a fan-triangulation algorithem
         */
		bool bol;
		FbxStringList lUVNames;
		msh->GetUVSetNames(lUVNames);

		FbxVector2 uv;
		if (lUVNames.GetCount() > 0)msh->GetPolygonVertexUV(j, 0, lUVNames[0], uv, bol);
		fbxsdk::FbxVector4* arr = msh->GetControlPoints();
		index = msh->GetPolygonVertex(j, 0);
		fbxsdk::FbxVector4 normal;
		msh->GetPolygonVertexNormal(j, 0, normal);
		double* ptr = (double*)arr[index];

		TGVertex first_vert, last_vert;

		addVertex(mesh, first_vert = {
				{ ptr[0], ptr[1], ptr[2]},
				color,
				{ uv[0], 1 - uv[1] },
				tex->index,
				{ normal[0], normal[1], normal[2]}
			});
		for (int i = 0; i < triangle_size; i++)
		{
			if (i == 0) {
				if (lUVNames.GetCount() > 0)msh->GetPolygonVertexUV(j, 1, lUVNames[0], uv, bol);
				index = msh->GetPolygonVertex(j, 1);
				ptr = (double*)arr[index];
				msh->GetPolygonVertexNormal(j, 1, normal);

				addVertex(mesh, {
						{ ptr[0], ptr[1], ptr[2]},
						color,
						{ uv[0], 1 - uv[1] },
						tex->index,
				{ normal[0], normal[1], normal[2]}
					});

				if (lUVNames.GetCount() > 0)msh->GetPolygonVertexUV(j, 2, lUVNames[0], uv, bol);
				index = msh->GetPolygonVertex(j, 2);
				ptr = (double*)arr[index];
				msh->GetPolygonVertexNormal(j, 2, normal);

				addVertex(mesh, last_vert = {
						{ ptr[0], ptr[1], ptr[2]},
						color,
						{ uv[0], 1 - uv[1] },
						tex->index,
				{ normal[0], normal[1], normal[2]}
					});
				continue;
			}

			addVertex(mesh, first_vert);
			addVertex(mesh, last_vert);

			if (lUVNames.GetCount() > 0)msh->GetPolygonVertexUV(j, i + 2, lUVNames[0], uv, bol);
			index = msh->GetPolygonVertex(j, i + 2);
			ptr = (double*)arr[index];
			msh->GetPolygonVertexNormal(j, i + 2, normal);

			addVertex(mesh, last_vert = {
					{ ptr[0], ptr[1], ptr[2]},
					color,
					{ uv[0], 1 - uv[1] },
					tex->index,
				{ normal[0], normal[1], normal[2]}
				});
		}
	}

}

uint32_t addVertex(Mesh* mesh, TGVertex vert)
{
	for (size_t b = 0; b < mesh->vertices.size(); b++)
	{
		if (mesh->vertices[b] == vert) {
			TG_VECTOR_APPEND_NORMAL(mesh->indices, (uint32_t)b)
			return (uint32_t)b;
		}
	}
	TG_VECTOR_APPEND_NORMAL(mesh->vertices, vert)
	uint32_t idx = (uint32_t)last_size;
	TG_VECTOR_APPEND_NORMAL(mesh->indices, idx)
	return idx;
}
