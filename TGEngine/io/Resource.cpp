#include "Resource.hpp"
#include "../io/Files.hpp"
#include "../gamecontent/Actor.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace tge::io {

	using namespace tge::nio;
	using namespace tge::tex;
	using namespace tge::gmc;

	static void loadResourceFileV2(File file, Map* map) {
		uint32_t blocklength = 0;
		uint32_t currentId = 0;

		uint32_t textureCount;
		fread(&textureCount, sizeof(uint32_t), 1, file);
		map->textures = new Texture[textureCount];
		TextureInputInfo* textureInputInfos = new TextureInputInfo[textureCount];

		fread(&blocklength, sizeof(uint32_t), 1, file);

		while (blocklength != UINT32_MAX) {
			stbi_uc* resbuffer = new stbi_uc[blocklength];
			fread(resbuffer, sizeof(stbi_uc), blocklength, file);

			// TODO Staging buffer for image memory

			TextureInputInfo* inputInfo = textureInputInfos + currentId;
			inputInfo->data = stbi_load_from_memory(resbuffer, (int)blocklength, &inputInfo->x, &inputInfo->y, &inputInfo->comp, STBI_rgb_alpha);

			fread(&blocklength, sizeof(uint32_t), 1, file);

			delete[] resbuffer;
			currentId++;
		}

		currentId = 0;

		fread(&blocklength, sizeof(uint32_t), 1, file);
		createdMaterials = new Material[blocklength];

		fread(&blocklength, sizeof(uint32_t), 1, file);

		while (blocklength != UINT32_MAX) {
			fread(&createdMaterials[currentId++], sizeof(uint8_t), blocklength, file);
			fread(&blocklength, sizeof(uint32_t), 1, file);
		}

		currentId = 0;
		
		uint32_t actorCount = 0;
		fread(&actorCount, sizeof(uint32_t), 1, file);

		// Start to read the actor
		ActorInputInfo* actorInputInfos = new ActorInputInfo[actorCount];

		// Read the block size of the following content
		fread(&blocklength, sizeof(uint32_t), 1, file);

		// Goes on until it hits a change request block size (2^32)
		while (blocklength != UINT32_MAX) {
			// Reads the actor properties and so on
			ActorInputInfo* actorInfo = actorInputInfos + currentId;

			fread(&actorInfo->pProperties, sizeof(tge::gmc::ActorProperties), 1, file);
			fread(&actorInfo->indexCount, sizeof(uint32_t), 1, file);
			fread(&actorInfo->vertexCount, sizeof(uint32_t), 1, file);

			/*
			 * TODO It would make sense to use staging buffer in this case to add the indices
			 * directly to the Graphics card
			 */

			 // Reads the indices from the file
			actorInfo->pIndices = new uint32_t[actorInfo->indexCount]; // Object lifetime ?
			fread(actorInfo->pIndices, sizeof(uint32_t), actorInfo->indexCount, file);

			// Reads the vertices
			actorInfo->pVertices = new uint8_t[blocklength]; // Object lifetime?
			fread(actorInfo->pVertices, sizeof(uint8_t), blocklength, file);

			// Read the next block size
			fread(&blocklength, sizeof(uint32_t), 1, file);
			currentId++;
		}

		fclose(file);

		SamplerInputInfo inputInfo;
		inputInfo.uSamplerMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		inputInfo.vSamplerMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		inputInfo.filterMagnification = VK_FILTER_NEAREST;
		inputInfo.filterMignification = VK_FILTER_NEAREST;

		createSampler(inputInfo, &map->sampler);

		createTextures(textureInputInfos, textureCount, map->textures);
		delete[] textureInputInfos;

		createActor(actorInputInfos, actorCount);
		delete[] actorInputInfos;
		return;
	}

#ifndef TGE_RESOURCE_LATEST_ONLY
	static void loadResourceFileV1(File file, Map* map) {
		uint32_t blocklength = 0;

		std::vector<TextureInputInfo> textureBindingInfos;
		textureBindingInfos.reserve(MAX_TEXTURES);

		fread(&blocklength, sizeof(uint32_t), 1, file);

		while (blocklength != UINT32_MAX) {
			stbi_uc* resbuffer = new stbi_uc[blocklength];
			fread(resbuffer, sizeof(stbi_uc), blocklength, file);

			TextureInputInfo inputInfo;
			inputInfo.data = stbi_load_from_memory(resbuffer, (int)blocklength, &inputInfo.x, &inputInfo.y, &inputInfo.comp, STBI_rgb_alpha);
			textureBindingInfos.push_back(inputInfo);

			fread(&blocklength, sizeof(uint32_t), 1, file);

			delete[] resbuffer;
		}

		blocklength = 0;

		fread(&blocklength, sizeof(uint32_t), 1, file);

		uint32_t materialID = 0;

		while (blocklength != UINT32_MAX) {
			fread(&createdMaterials[materialID++], sizeof(uint8_t), blocklength, file);
			fread(&blocklength, sizeof(uint32_t), 1, file);
		}

		// Start to read the actor
		std::vector<ActorInputInfo> actorInputInfos;
		actorInputInfos.reserve(MAX_TEXTURES); // This is speculativ and could be any number
		// I just happen to take the texture number

		// Read the block size of the following content
		fread(&blocklength, sizeof(uint32_t), 1, file);

		// Goes on until it hits a change request block size (2^32)
		while (blocklength != UINT32_MAX) {
			// Reads the actor properties and so on
			ActorInputInfo actorInfo;

			fread(&actorInfo.pProperties, sizeof(tge::gmc::ActorProperties), 1, file);
			fread(&actorInfo.indexCount, sizeof(uint32_t), 1, file);
			fread(&actorInfo.vertexCount, sizeof(uint32_t), 1, file);
			// 2x4 + 2 + 4x16

			/*
			 * It would make sense to use staging buffer in this case to add the indices
			 * directly to the Graphics card
			 */

			 // Reads the indices from the file
			actorInfo.pIndices = new uint32_t[actorInfo.indexCount]; // Object lifetime ?
			fread(actorInfo.pIndices, sizeof(uint32_t), actorInfo.indexCount, file);

			// Reads the vertices
			actorInfo.pVertices = new uint8_t[blocklength]; // Object lifetime?
			fread(actorInfo.pVertices, sizeof(uint8_t), blocklength, file);

			actorInputInfos.push_back(actorInfo);

			// Read the next block size
			fread(&blocklength, sizeof(uint32_t), 1, file);
		}

		fclose(file);

		SamplerInputInfo inputInfo;
		inputInfo.uSamplerMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		inputInfo.vSamplerMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		inputInfo.filterMagnification = VK_FILTER_NEAREST;
		inputInfo.filterMignification = VK_FILTER_NEAREST;

		createSampler(inputInfo, &map->sampler);

		map->textures = new Texture[textureBindingInfos.size()];
		createTextures(textureBindingInfos.data(), (uint32_t)textureBindingInfos.size(), map->textures);

		createActor(actorInputInfos.data(), (uint32_t)actorInputInfos.size());
		return;
	}
#endif

	void loadResourceFile(const char* name, Map* map) {
		File file = open(name, "rb");

		uint32_t header = 0;
		fread(&header, sizeof(uint32_t), 1, file);

#ifdef DEBUG
		if (header != TGR_LATEST)
			OUT_LV_DEBUG("Warning: you are using an old version of the tgr file, consider updating!")
#endif // DEBUG

#ifndef TGE_RESOURCE_LATEST_ONLY
			if (header == TGR_VERSION_1) {
				loadResourceFileV1(file, map);
			}
			else
#endif // !TGE_RESOURCE_LATEST_ONLY
				if (header == TGR_VERSION_2) {
					loadResourceFileV2(file, map);
				}
				else {
					OUT_LV_DEBUG("Header does not match with the parser versions, skipping!");
					return;
				}
	}
}