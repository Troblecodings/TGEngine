#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include "../util/Debug.hpp"
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include "../stb/stb_truetype.h"
#include "../drawlib/Rectangle.hpp"
#include "../io/Files.hpp"
#include "../game_content/Material.hpp"

namespace tg_font {

	using namespace tge::tex;

	SINCE(0, 0, 4)
	class Font {

	private:
		uint32_t height;
		Texture* texture;
		stbtt_bakedchar cdata[256];
		uint32_t materialIndex;

	public:
		/*
		* Initializes all values (besides default constructor)
		* Adds new font to the cache and adds the generated texture +  it's material (UI)
		*/
		Font(char* path) : Font(path, 30) {}
		Font(char* path, uint32_t height);

		/*
		* Draws a string to the screen (UI only)
		*/
		SINCE(0, 0, 3)
		void drawString(glm::vec2 pos, char* string, VertexBuffer* buffer, IndexBuffer* ibuffer);

		SINCE(0, 0, 4)
		uint32_t getMaterialIndex();

		SINCE(0, 0, 4)
		glm::vec2 getExtent(char* chr);
	};
}