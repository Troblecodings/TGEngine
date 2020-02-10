#include "Resource.hpp"
#include "../io/Files.hpp"
#include "../gamecontent/Actor.hpp"

using namespace tge::nio;
using namespace tge::tex;
using namespace tge::gmc;

Map loadResourceFile(char* name) {
	File file = open(name, "r");

	uint32_t header = 0;
	fread(&header, sizeof(uint32_t), 1, file);

	Map map;

	if (header != TGR_VERSION_1) {
		OUT_LV_DEBUG("Header does not match with the parser version!")
		return map;
	}

	if (feof(file) != 0) {
		OUT_LV_DEBUG("File corrupted!")
		return map;
	}

	uint32_t blocklength = 0;

	std::vector<TextureInputInfo> textureInfos;
	textureInfos.reserve(2048);

	while (blocklength != UINT32_MAX)
	{
		fread(&blocklength, sizeof(uint32_t), 1, file);

		stbi_uc* resbuffer = new stbi_uc[blocklength];
		fread(resbuffer, sizeof(char), blocklength, file);

		TextureInputInfo inputInfo;
		inputInfo.data = stbi_load_from_memory(resbuffer, (int)blocklength, &inputInfo.x, &inputInfo.y, &inputInfo.comp, STBI_rgb_alpha);
		textureInfos.push_back(inputInfo);
	}

	TextureBindingInfo bindingInfo[2048];
	map.textures = new Texture[textureInfos.size()];
	createTextures(textureInfos.data(), (uint32_t)textureInfos.size(), map.textures, bindingInfo);

	if (feof(file) != 0)
		return map;

	blocklength = 0;

	std::vector<Material> materials;
	materials.reserve(2048);

	while (blocklength != UINT32_MAX)
	{
		fread(&blocklength, sizeof(uint32_t), 1, file);

		Material material;
		fread(&material, sizeof(Material), 1, file);

		materials.push_back(material);
	}

	if (feof(file) != 0)
		return map;

	std::vector<ActorInputInfo> actors;
	actors.reserve(2048);

	while (blocklength != UINT32_MAX)
	{
		fread(&blocklength, sizeof(uint32_t), 1, file);

		ActorInputInfo actorInfo;
		fread(&actorInfo, sizeof(ActorInputInfo), 1, file);

		actorInfo.pIndices = new uint32_t[actorInfo.indexCount]; // Object lifetime ?
		fread(&actorInfo.pIndices, sizeof(uint32_t), actorInfo.indexCount, file);

		uint32_t len = blocklength - (sizeof(ActorInputInfo) + sizeof(uint32_t) * actorInfo.indexCount);

		actorInfo.pVertices = new uint8_t[len]; // Object lifetime?
		fread(actorInfo.pVertices, sizeof(uint8_t), len, file);

		actors.push_back(actorInfo);
	}

	createActor(actors.data(), (uint32_t)actors.size());

	return map;
}