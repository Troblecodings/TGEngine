#include "FBX_Loader.hpp"

namespace tg_model {

	INTERNAL
	static void readVertex(FbxAMatrix matrix, FbxMesh* fbxmesh, TGVertex* vert, Material* mat, FbxStringList lUVNames, uint32_t j, uint32_t i) {
		FbxVector2 uv;
		bool uv_vert;
		fbxsdk::FbxVector4 normal;

		fbxmesh->GetPolygonVertexUV(j, i, lUVNames[0], uv, uv_vert);
		uint32_t index = fbxmesh->GetPolygonVertex(j, i);
		FbxDouble4 ptr = matrix.MultT(fbxmesh->GetControlPoints()[index]);
		fbxmesh->GetPolygonVertexNormal(j, i, normal);

		vert->position = glm::vec4( ptr[0], ptr[1], ptr[2], 1);
		vert->color = mat->color;
		vert->uv = { uv[0], 1 - uv[1] };
		if(mat->texture) vert->color_only = mat->texture->index;
		else vert->color_only = COLOR_ONLY;
		vert->normal = { normal[0], normal[1], normal[2] };
	}

	static FbxAMatrix geometricoffset(FbxNode* pNode)
	{
		const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

		return FbxAMatrix(lT, lR, lS);
	}

	INTERNAL
	static void addMesh(char* name, FbxNode* node, Mesh* mesh) {
		ASSERT_NONE_NULL_DB(node, "Fbx node null in mesh " << name, TG_ERR_DB_NULLPTR)
		if (!node || !mesh) return;

		FbxMesh* fbxmesh = node->GetMesh();

		if (fbxmesh) {
			
			FbxAMatrix matrix0;
			matrix0.SetIdentity();
			if (node->GetNodeAttribute())
			{
				const FbxVector4 lT = node->GetGeometricTranslation(FbxNode::eSourcePivot);
				const FbxVector4 lR = node->GetGeometricRotation(FbxNode::eSourcePivot);
				const FbxVector4 lS = node->GetGeometricScaling(FbxNode::eSourcePivot);
				matrix0.SetT(lT);
				matrix0.SetR(lR);
				matrix0.SetS(lS);
			}
			FbxAMatrix localMatrix = node->EvaluateLocalTransform();

			FbxNode* pParentNode = node->GetParent();
			FbxAMatrix parentMatrix = pParentNode->EvaluateLocalTransform();
			while ((pParentNode = pParentNode->GetParent()) != NULL)
			{
				parentMatrix = pParentNode->EvaluateLocalTransform() * parentMatrix;
			}
			FbxAMatrix matrix = parentMatrix * localMatrix * matrix0;

			FbxStringList lUVNames;
			fbxmesh->GetUVSetNames(lUVNames);
			if (lUVNames.GetCount() < 1) {
				OUT_LV_DEBUG("No UV map in fbxmodel[" << name << "]")
				for (size_t i = 0; i < node->GetChildCount(); i++)
					addMesh(name, node->GetChild(i), mesh);
				return;
			}

			Material material = { nullptr };

			FbxSurfaceLambert* surface_lambert = (FbxSurfaceLambert*)node->GetMaterial(0);
			if (surface_lambert) {
				FbxDouble3 color_data = surface_lambert->Diffuse.Get();
				material.color = { (float)color_data[0], (float)color_data[1], (float)color_data[2], (float)(1 - surface_lambert->TransparencyFactor.Get()) };
				fbxsdk::FbxObject* object = surface_lambert->Diffuse.GetSrcObject();
				if (object) {
					fbxsdk::FbxFileTexture* tex = (fbxsdk::FbxFileTexture*)object;
					if (tex && tex->GetFileName() != nullptr) {
						material.texture = new Texture();
						material.texture->texture_path = (char*) tex->GetFileName();
						createTexture(material.texture);
					}
					else {
						OUT_LV_DEBUG("Src object not a texture in fbxmodel[" << name << "]")
					}
				}
				else {
					OUT_LV_DEBUG("No src object defined in fbxmodel[" << name << "]")
				}
			}
			else {
				OUT_LV_DEBUG("No surface_lambert material defined in fbxmodel[" << name << "]")
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
				readVertex(matrix, fbxmesh, &first_vert, &material, lUVNames, j, 0);

				for (int i = 0; i < triangle_size; i++)
				{
					if (i == 0) {

						mesh->add(first_vert);
						readVertex(matrix, fbxmesh, &last_vert, &material, lUVNames, j, 1);
						mesh->add(last_vert);
						readVertex(matrix, fbxmesh, &last_vert, &material, lUVNames, j, 2);
						mesh->add(last_vert);
						continue;
					}

					mesh->add(first_vert);
					mesh->add(last_vert);

					readVertex(matrix, fbxmesh, &last_vert, &material, lUVNames, j, i + 2);
					mesh->add(last_vert);
				}
			}
		}
		for (size_t i = 0; i < node->GetChildCount(); i++)
			addMesh(name, node->GetChild(i), mesh);
	}

	void load(char* name, Mesh* mesh) {
		FbxManager* manager = FbxManager::Create();

		FbxIOSettings* io_settings = FbxIOSettings::Create(manager, IOSROOT);
		manager->SetIOSettings(io_settings);

		FbxImporter* importer = FbxImporter::Create(manager, "");
		FBX_CHECK(importer->Initialize(name, -1, manager->GetIOSettings()))

		FbxScene* scene = FbxScene::Create(manager, "");
		FBX_CHECK(importer->Import(scene))

		importer->Destroy();

		FbxNode* fbx_node = scene->GetRootNode();
		addMesh(name, fbx_node, mesh);
	}

}