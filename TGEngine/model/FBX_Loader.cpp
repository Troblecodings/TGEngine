#include "FBX_Loader.hpp"

namespace tg_model {

	INTERNAL
	static void readVertex(FbxAMatrix matrix, FbxMesh* fbxmesh, TGVertex* vert, FbxStringList lUVNames, uint32_t j, uint32_t i) {
		FbxVector2 uv;
		bool uv_vert;
		fbxsdk::FbxVector4 normal;

		fbxmesh->GetPolygonVertexUV(j, i, lUVNames[0], uv, uv_vert);
		uint32_t index = fbxmesh->GetPolygonVertex(j, i);
		FbxDouble4 ptr = matrix.MultT(fbxmesh->GetControlPoints()[index]);
		fbxmesh->GetPolygonVertexNormal(j, i, normal);

		vert->position = glm::vec4( ptr[0], ptr[1], ptr[2], 1);
		vert->uv = { uv[0], 1 - uv[1] };
		vert->normal = { normal[0], normal[1], normal[2] };
	}

	INTERNAL
	static void addMesh(char* name, FbxNode* node, Mesh* mesh) {
		ASSERT_NONE_NULL_DB(node, "Fbx node null in mesh " << name, TG_ERR_DB_NULLPTR)
		if (!node || !mesh) return;

		OUT_LV_DEBUG("New node [" << node->GetName() << "]")

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
				for (int i = 0; i < node->GetChildCount(); i++)
					addMesh(name, node->GetChild(i), mesh);
				return;
			}

			// TODO test material
			// Maybe different shaders
			FbxSurfaceLambert* surface_lambert = (FbxSurfaceLambert*)node->GetMaterial(0);
			Texture* texture;
			glm::vec4 color;
			if (surface_lambert) {
				FbxDouble3 color_data = surface_lambert->Diffuse.Get();
				color = { (float)color_data[0], (float)color_data[1], (float)color_data[2], (float)(1 - surface_lambert->TransparencyFactor.Get()) };
				fbxsdk::FbxObject* object = surface_lambert->Diffuse.GetSrcObject();
				if (object) {
					fbxsdk::FbxFileTexture* tex = (fbxsdk::FbxFileTexture*)object;
					if (tex && tex->GetFileName() != nullptr) {
						texture = new Texture(const_cast<char*>(tex->GetFileName()));
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

			TGVertex last_vert;
			RenderOffsets offsets;
			offsets.offset = (uint32_t)mesh->indices.size();
			for (int j = 0; j < fbxmesh->GetPolygonCount(); j++) {
				int triangle_size = fbxmesh->GetPolygonSize(j);
				if(triangle_size == 3) {
					for (uint32_t i = 0; i < 3; i++)
					{
						readVertex(matrix, fbxmesh, &last_vert, lUVNames, j, i);
						mesh->add(last_vert);
					}
				}
			}
			Material mat = Material(texture, color);
			TG_VECTOR_APPEND_NORMAL(materials, &mat)
			mesh->materials.push_back(offsets.material = (uint32_t)last_size);
			offsets.size = (uint32_t)mesh->indices.size() - offsets.offset;
			mesh->offsets.push_back(offsets);
		}
		for (int i = 0; i < node->GetChildCount(); i++)
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

		FbxGeometryConverter converter(manager);
		converter.Triangulate(scene, true);

		FbxNode* fbx_node = scene->GetRootNode();
		addMesh(name, fbx_node, mesh);
	}

}