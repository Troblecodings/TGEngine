#include "Font.hpp"
#include "../gamecontent/camera/Camera.hpp"
#include "Files.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"

namespace tge {
	namespace fnt {
		void readFontfile(const char* name, const float fontheight, tge::tex::TextureInputInfo* textureInputInfo) {
			long size = 0;
			textureInputInfo->data = new uint8_t[16000000];
			stbtt_bakedchar charData[256];
			textureInputInfo->x = 2000;
			textureInputInfo->y = 2000;
			textureInputInfo->comp = 4;
			uint8_t* filedata = tge::nio::readAll(name, &size);
			stbtt_BakeFontBitmap(filedata, 0, fontheight, textureInputInfo->data, 2000, 2000, 0, 256, charData);
			
		}
	}
}