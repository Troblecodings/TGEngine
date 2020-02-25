#include "Resource.hpp"
#include "../io/Files.hpp"
#include "../gamecontent/Actor.hpp"
#include <glm/gtc/type_ptr.hpp>

using namespace tge::nio;
using namespace tge::tex;
using namespace tge::gmc;

void loadResourceFile(const char* name, Map* map) {
	File file = open(name, "rb");

	uint32_t header = 0;
	fread(&header, sizeof(uint32_t), 1, file);

	if (header != TGR_VERSION_1) {
		OUT_LV_DEBUG("Header does not match with the parser version!")
		return;
	}

	uint32_t blocklength = 0;

	std::vector<TextureInputInfo> textureBindingInfos;
	textureBindingInfos.reserve(2048);

	fread(&blocklength, sizeof(uint32_t), 1, file);

	while (blocklength != UINT32_MAX)
	{
		stbi_uc* resbuffer = new stbi_uc[blocklength];
		fread(resbuffer, sizeof(stbi_uc), blocklength, file);

		TextureInputInfo inputInfo;
		inputInfo.data = stbi_load_from_memory(resbuffer, (int)blocklength, &inputInfo.x, &inputInfo.y, &inputInfo.comp, STBI_rgb_alpha);
		textureBindingInfos.push_back(inputInfo);

		fread(&blocklength, sizeof(uint32_t), 1, file);
	}

	blocklength = 0;

	createdMaterials = new Material[256];

	fread(&blocklength, sizeof(uint32_t), 1, file);

	uint32_t materialID = 0;

	while (blocklength != UINT32_MAX)
	{
		Material material;
		fread(&material, sizeof(Material), 1, file);

		createdMaterials[materialID++] = material;

		fread(&blocklength, sizeof(uint32_t), 1, file);
	}

	// Start to read the actor
	std::vector<ActorInputInfo> actorInputInfos;
	actorInputInfos.reserve(2048);

	// Read the block size of the following content
	fread(&blocklength, sizeof(uint32_t), 1, file);

	// Goes on until it hits a change request block size (2^32)
	while (blocklength != UINT32_MAX)
	{
		// Reads the actor properties and so on
		ActorInputInfo actorInfo;

		// Sadly we cannot reduce read calls and have to do this all with manually
		float transformMatrix[16];
		fread(&transformMatrix, sizeof(float), 16, file);
		actorInfo.pProperties.localTransform = glm::make_mat4(transformMatrix);

		fread(&actorInfo.pProperties.material, sizeof(uint8_t), 1, file);
		fread(&actorInfo.pProperties.layer, sizeof(uint8_t), 1, file);
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

	map->textures.resize(textureBindingInfos.size());
	createTextures(textureBindingInfos.data(), (uint32_t)textureBindingInfos.size(), map->textures.data());

	createActor(actorInputInfos.data(), (uint32_t)actorInputInfos.size());
	return;
}
