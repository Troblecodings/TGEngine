#include "FBX_Loader.hpp"

namespace tg_model {

	INTERNAL
	static void readVertex(FbxMesh* fbxmesh, TGVertex* vert, Material* mat, FbxStringList lUVNames, uint32_t j, uint32_t i) {
		FbxVector2 uv;
		bool uv_vert;
		fbxsdk::FbxVector4 normal;

		fbxmesh->GetPolygonVertexUV(j, i, lUVNames[0], uv, uv_vert);
		uint32_t index = fbxmesh->GetPolygonVertex(j, i);
		double* ptr = (double*)fbxmesh->GetControlPoints()[index];
		fbxmesh->GetPolygonVertexNormal(j, i, normal);

		vert->position = { ptr[0], ptr[1], ptr[2] };
		vert->color = mat->color;
		vert->uv = { uv[0], 1 - uv[1] };
		if(mat->texture) vert->color_only = mat->texture->index;
		else vert->color_only = COLOR_ONLY;
		vert->normal = { normal[0], normal[1], normal[2] };
	}

	INTERNAL
	static void addMesh(char* name, FbxNode* node, uint32_t* count, Mesh* mesh) {
		ASSERT_NONE_NULL_DB(node, "Fbx node null in mesh " << name, TG_ERR_DB_NULLPTR)

		FbxMesh* fbxmesh = node->GetMesh();
		if (fbxmesh && mesh) {
			Renderer* render = new Renderer();
			render->vbuffer = &mesh[*count].vertices;
			render->ibuffer = &mesh[*count].indices;

			FbxStringList lUVNames;
			fbxmesh->GetUVSetNames(lUVNames);
			if (lUVNames.GetCount() > 0) return;

			Material material = { nullptr };

			FbxSurfaceLambert* surface_lambert = (FbxSurfaceLambert*)node->GetMaterial(0);
			if (surface_lambert) {
				if (fbxsdk::FbxDouble* color_data = surface_lambert->Diffuse.Get().mData) {
					material.color = { (float)color_data[0], (float)color_data[1], (float)color_data[2], (float)(1 - surface_lambert->TransparencyFactor.Get()) };
				}
				fbxsdk::FbxObject* object = surface_lambert->Diffuse.GetSrcObject();
				if (object) {
					fbxsdk::FbxFileTexture* tex = (fbxsdk::FbxFileTexture*)object;
					if (tex && tex->GetFileName() != nullptr) {
						material.texture = new Texture();
						material.texture->texture_path = (char*) tex->GetFileName();
						createTexture(material.texture);
					}
				}
			}

			/*
             * Proccess data
             */
			for (int j = 0; j < fbxmesh->GetPolygonCount(); j++) {
				int triangle_size = fbxmesh->GetPolygonSize(j);
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
				TGVertex first_vert, last_vert;
				readVertex(fbxmesh, &first_vert, &material, lUVNames, j, 0);

				for (int i = 0; i < triangle_size; i++)
				{
					if (i == 0) {

						readVertex(fbxmesh, &last_vert, &material, lUVNames, j, 1);
						last_vert >> render;
						readVertex(fbxmesh, &last_vert, &material, lUVNames, j, 2);
						last_vert >> render;
						continue;
					}

					first_vert >> render;
					last_vert >> render;

					readVertex(fbxmesh, &last_vert, &material, lUVNames, j, i + 2);
					last_vert >> render;
				}
			}
		}
		if (count)
			count++;
		for (size_t i = 0; i < node->GetChildCount(); i++)
			addMesh(name, node->GetChild(i), count, mesh);
	}

	void load(char* name, uint32_t* count, Mesh* mesh) {
		FbxManager* manager = FbxManager::Create();

		FbxIOSettings* io_settings = FbxIOSettings::Create(manager, IOSROOT);
		manager->SetIOSettings(io_settings);

		FbxImporter* importer = FbxImporter::Create(manager, "");
		FBX_CHECK(importer->Initialize(name, -1, manager->GetIOSettings()))

		FbxScene* scene = FbxScene::Create(manager, "");
		FBX_CHECK(importer->Import(scene))

		importer->Destroy();

		for (size_t i = 0; i < scene->GetNodeCount(); i++)
		{
			FbxNode* fbx_node = scene->GetNode(i);
			addMesh(name, fbx_node, count, mesh);
		}
	}

}