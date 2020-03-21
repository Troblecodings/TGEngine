#pragma once

#include <vector>
#include <iostream>
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include "../stb/stb_truetype.h"
#include "../io/Files.hpp"
#include "../gamecontent/Actor.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"

namespace tge::fnt {

		constexpr uint32_t FONT_TEXTURE_WIDTH = 2000;
		constexpr uint32_t FONT_TEXTURE_HEIGHT = 2000;

		struct Font {
			uint32_t material;
			stbtt_bakedchar* charData;
			float fontheight; // Primarily for alignment
		};

		extern Font* fonts;

#if 0
		void createStringActor(const char* inputString, Font* font, tge::gmc::ActorInputInfo* actor);
#endif // DEBUG

}
