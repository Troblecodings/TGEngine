#include "Resource.hpp"
#include "../io/Files.hpp"
#include "../gamecontent/Actor.hpp"

using namespace tge::nio;
using namespace tge::tex;
using namespace tge::gmc;

void loadResourceFile(const char* name, Map* map) {
	File file = open(name, "r");

	uint32_t header = 0;
	fread(&header, sizeof(uint32_t), 1, file);

	if (header != TGR_VERSION_1) {
		OUT_LV_DEBUG("Header does not match with the parser version!")
		return;
	}

	uint32_t blocklength = 0;

	map->textures.reserve(2048);

	fread(&blocklength, sizeof(uint32_t), 1, file);

	while (blocklength != UINT32_MAX)
	{
		stbi_uc* resbuffer = new stbi_uc[blocklength];
		fread(resbuffer, sizeof(char), blocklength, file);

		TextureInputInfo inputInfo;
		inputInfo.data = stbi_load_from_memory(resbuffer, (int)blocklength, &inputInfo.x, &inputInfo.y, &inputInfo.comp, STBI_rgb_alpha);
		map->textures.push_back(inputInfo);

		fread(&blocklength, sizeof(uint32_t), 1, file);
	}

	if (feof(file) != 0)
		return;

	blocklength = 0;

	map->materials.reserve(256);

	fread(&blocklength, sizeof(uint32_t), 1, file);

	while (blocklength != UINT32_MAX)
	{
		Material material;
		fread(&material, sizeof(Material), 1, file);

		map->materials.push_back(material);

		fread(&blocklength, sizeof(uint32_t), 1, file);
	}

	if (feof(file) != 0)
		return;

	// Start to read the actor
	map->actors.reserve(2048);

	// Read the block size of the following content
	fread(&blocklength, sizeof(uint32_t), 1, file);

	// Goes on until it hits a change request block size (2^32)
	while (blocklength != UINT32_MAX)
	{
		// Reads the actor properties and so on
		ActorInputInfo actorInfo;
		fread(&actorInfo, sizeof(ActorInputInfo), 1, file);


		/*
		 * It would make sense to use staging buffer in this case to add the indicies 
		 * directly to the Graphics card
		 */

		// Reads the indicies from the file
		actorInfo.pIndices = new uint32_t[actorInfo.indexCount]; // Object lifetime ?
		fread(&actorInfo.pIndices, sizeof(uint32_t), actorInfo.indexCount, file);

		// calculates the length of the verteicies to read
		uint32_t len = blocklength - (sizeof(ActorInputInfo) + sizeof(uint32_t) * actorInfo.indexCount);

		// Reads the verticies
		actorInfo.pVertices = new uint8_t[len]; // Object lifetime?
		fread(actorInfo.pVertices, sizeof(uint8_t), len, file);

		// Not sure about object lifetime here ... problem could be that the objects are being deleted
		// if the method exits. However those should be part of the ouput
		// which is returned ... this hover is only a local variable
		// Needs research

		map->actors.push_back(actorInfo);

		// Read the next block size
		fread(&blocklength, sizeof(uint32_t), 1, file);
	}

	createActor(map->actors.data(), (uint32_t)map->actors.size());

	return;
}