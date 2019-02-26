#include "FBX_Loader.hpp"

namespace FBX_Dictionary {
	std::vector<fbxsdk::FbxMesh*> fbx_meshes = {};
	std::vector<char*> names = {};
	std::vector<glm::vec4> colors = {};
	std::vector<Texture> textures = {};

	void addToDraw(VertexBuffer* buffer) {
		for (size_t r = 0; r < FBX_Dictionary::names.size(); r++)
		{
			FbxMesh* mesh = FBX_Dictionary::fbx_meshes[r];
			FbxStringList lUVNames;
			mesh->GetUVSetNames(lUVNames);
			Texture* tex = &FBX_Dictionary::textures[r];
			FbxVector4* vertexArray = mesh->GetControlPoints();
			glm::vec4 color = FBX_Dictionary::colors[r];

			if (tex) {

				for (int j = 0; j < mesh->GetPolygonCount(); j++) {
					for (int i = 0; i < mesh->GetPolygonSize(j); i++) {
						int index = mesh->GetPolygonVertex(j, i);
						fbxsdk::FbxVector2 vector;
						glm::vec2 uv = {};
						bool unmapped;
						if (mesh->GetPolygonVertexUV(j, i, lUVNames[0], vector, unmapped)) {
							uv = { (float)vector.mData[0], 1 - (float)vector.mData[1] };
						}
						FbxDouble* data = vertexArray[index].mData;
						buffer->add({ { (float)data[0], (float)data[1] , (float)data[2] }, { color[0], color[1], color[2], color[3] }, { uv[0], uv[1] }, tex->index });
					}
				}
			}
			else {
				tex->index = COLOR_ONLY;
				for (int j = 0; j < mesh->GetPolygonCount(); j++) {
					for (int i = 0; i < mesh->GetPolygonSize(j); i++) {
						int index = mesh->GetPolygonVertex(j, i);
						fbxsdk::FbxVector2 vector;
						glm::vec2 uv = {};
						bool unmapped;
						if (mesh->GetPolygonVertexUV(j, i, lUVNames[0], vector, unmapped)) {
							uv = { (float)vector.mData[0], 1 - (float)vector.mData[1] };
						}
						FbxDouble* data = vertexArray[index].mData;
						buffer->add({ { (float)data[0], (float)data[1] , (float)data[2] },{ color[0], color[1], color[2], color[3] },{ uv[0], uv[1] }, COLOR_ONLY });
					}
				}
			}
		}
	}

	uint32_t addAll(fbxsdk::FbxNode* node)
	{
		fbxsdk::FbxMesh* mesh = node->GetMesh();
		size_t pos = FBX_Dictionary::names.size();
		if (mesh) {
			fbxsdk::FbxSurfaceMaterial* material = node->GetMaterial(0);

			glm::vec4 color = { 0, 0, 0, 1 };

			FBX_Dictionary::names.resize(pos + 1);
			FBX_Dictionary::fbx_meshes.resize(pos + 1);
			FBX_Dictionary::colors.resize(pos + 1);
			FBX_Dictionary::textures.resize(pos + 1);

			if (material) {
				fbxsdk::FbxSurfaceLambert* surface_lambert = (fbxsdk::FbxSurfaceLambert*)(material);
				if (surface_lambert) {
					if (fbxsdk::FbxDouble* color_data = surface_lambert->Diffuse.Get().mData) {
						color = { (float)color_data[0], (float)color_data[1], (float)color_data[2], (float)(1 - surface_lambert->TransparencyFactor.Get()) };
					}
					fbxsdk::FbxObject* object = surface_lambert->Diffuse.GetSrcObject();
					if (object) {
						fbxsdk::FbxFileTexture* tex = (fbxsdk::FbxFileTexture*)object;
						if (tex && tex->GetFileName() != nullptr) {
							FBX_Dictionary::textures[pos] = {
								(char*)tex->GetFileName()
							};
							createTexture(&FBX_Dictionary::textures[pos]);
						}
						else {
							FBX_Dictionary::textures[pos].texture_path = nullptr;
							FBX_Dictionary::textures[pos].index = COLOR_ONLY;
						}
					}
					else {
						FBX_Dictionary::textures[pos].texture_path = nullptr;
						FBX_Dictionary::textures[pos].index = COLOR_ONLY;
					}
				}
			}
			FBX_Dictionary::colors[pos] = color;
			FBX_Dictionary::fbx_meshes[pos] = mesh;
		}

		for (int i = 0; i < node->GetChildCount(); i++)
			addAll(node->GetChild(i));
		return (uint32_t)pos;
	}
};


uint32_t load(char* name) {
	FbxManager* manager = FbxManager::Create();

	FbxIOSettings* io_settings = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(io_settings);

	FbxImporter* importer = FbxImporter::Create(manager, "");
	FBX_CHECK(importer->Initialize(name, -1, manager->GetIOSettings()))

	FbxScene* scene = FbxScene::Create(manager, "");
	FBX_CHECK(importer->Import(scene))

	importer->Destroy();

	FbxNode* fbx_node = scene->GetRootNode();
	ASSERT_NONE_NULL_DB(fbx_node, "Fbx node null in mesh " << name, TG_ERR_DB_NULLPTR)

	return FBX_Dictionary::addAll(fbx_node);
}