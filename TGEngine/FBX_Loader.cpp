#include "FBX_Loader.hpp"

void load(char* name, FbxMesh* mesh) {
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
	importer->Import(scene);
	importer->Destroy();

	FbxNode* fbx_node = scene->GetRootNode();
	
	mesh = fbx_node->GetMesh();
}

void addToDraw(FbxMesh* mesh, VertexBuffer* buffer) {
	int vertexCount = mesh->GetControlPointsCount();
	FbxVector4* vertexArray = mesh->GetControlPoints();
	for (size_t i = 0; i < vertexCount; i++) {
		FbxDouble* data = vertexArray[i].mData;
		buffer->add({ {(float)data[0], (float)data[0] , (float)data[0]}, {1, 1, 0, 1} });
	}
}