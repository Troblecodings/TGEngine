#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include "../util/Debug.hpp"
#include "../pipeline/Buffer/Texturebuffer.hpp"
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include "../stb/stb_truetype.h"
#include "../drawlib/Rectangle.hpp"
#include "../io/Files.hpp"

namespace tg_font {

	SINCE(0, 0, 4)
	class Font {

		public:
			/*
			 * Initializes all values (besides default constructor)
			 * Adds new font to the cache and adds the generated texture +  it's material (UI)
			 */
			Font() {}
			Font(char* path) : Font(path, 30) {}
			Font(char* path, int height);

			/*
			 * Draws a string to the screen (UI only)
			 */
			SINCE(0, 0, 3)
			float drawString(glm::vec3 pos, glm::vec4 color, char* string, VertexBuffer* buffer, IndexBuffer* ibuffer, float multi = 0.0015);

		private:
			Texture texture;
			stbtt_bakedchar cdata[256];
			uint32_t material;
	};
}