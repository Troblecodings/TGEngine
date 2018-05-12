#include "FBX_Loader.hpp"

fbxsdk::FbxNode* load(char* name) {
	FbxManager* manager = FbxManager::Create();

	FbxIOSettings* io_settings = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(io_settings);

	FbxImporter* importer = FbxImporter::Create(manager, "");
	bool success = importer->Initialize(name, -1, manager->GetIOSettings());

	if (!success) {
		OUT_LV_DEBUG("Failed to load " << name)
		OUT_LV_DEBUG("Error: " << importer->GetStatus().GetErrorString())
	}

	FbxScene* scene = FbxScene::Create(manager, "");
	success = importer->Import(scene);

	if (!success) {
		OUT_LV_DEBUG("Failed to load " << name)
		OUT_LV_DEBUG("Error: " << importer->GetStatus().GetErrorString())
	}

	importer->Destroy();

	FbxNode* fbx_node = scene->GetRootNode();

	if (!fbx_node) {
		OUT_LV_DEBUG("Failed to load " << name)
		OUT_LV_DEBUG("Error: " << importer->GetStatus().GetErrorString())
	}
	return fbx_node;
}

void addToDraw(fbxsdk::FbxNode* node, VertexBuffer* buffer) {

	FbxMesh* mesh = node->GetMesh();
	 
	if (mesh) {
		FbxVector4* vertexArray = mesh->GetControlPoints();
		for(size_t j = 0;j < mesh->GetPolygonCount(); j++) {
			size_t aut = 0;
			for (size_t i = 0; i < mesh->GetPolygonSize(j); i++) {
				int index = mesh->GetPolygonVertex(j, i);
				FbxDouble* data = vertexArray[index].mData;
				buffer->add({ {(float)data[0], (float)data[1] , (float)data[2]}, {1, 1, 0, 1}, {0, 0}, VK_TRUE });
			}
		}
	}

	for (int j = 0; j < node->GetChildCount(); j++)
		addToDraw(node->GetChild(j), buffer);
}