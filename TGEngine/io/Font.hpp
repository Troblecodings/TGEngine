#pragma once

#include <vector>
#include <iostream>
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include "../stb/stb_truetype.h"
#include "../io/Files.hpp"
#include "../gamecontent/Actor.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"

namespace tge {
	namespace fnt {

		constexpr uint32_t FONT_TEXTURE_WIDTH = 2000;
		constexpr uint32_t FONT_TEXTURE_HEIGHT = 2000;

		void readFontfile(const char* name, const float fontheight, tge::tex::TextureInputInfo* textureInputInfo, stbtt_bakedchar* charData);

		void createStringActor(const char* inputString, const stbtt_bakedchar* charData, tge::gmc::ActorInputInfo* actor);

	}
}