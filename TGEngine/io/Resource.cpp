#include "Resource.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"
#include "../io/Files.hpp"

using namespace tge::nio;
using namespace tge::tex;

void loadResourceFile(char* name) {
	File file = open(name, "r");

	uint32_t header = 0;
	fread(&header, sizeof(uint32_t), 1, file);

	if (header != TGR_VERSION_1) {
		OUT_LV_DEBUG("Header does not match with the parser version!")
		return;
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
}