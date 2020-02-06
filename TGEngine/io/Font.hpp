#pragma once

#include <vector>
#include <iostream>
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include "../stb/stb_truetype.h"
#include "../io/Files.hpp"
#include "../util/VectorUtil.hpp"
#include "../pipeline/buffer/VertexBuffer.hpp"
#include "../pipeline/buffer/IndexBuffer.hpp"

namespace tge {
	namespace fnt {

		/*
		 * <strong>This API is deprecated!</strong>
		 */
		class Font {

		private:
			uint32_t height = 20;
			stbtt_bakedchar cdata[256];
			uint32_t materialIndex = -1;

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
			void drawString(glm::vec2 pos, char* string, VertexBuffer* buffer, IndexBuffer* ibuffer, glm::vec4 color);

			uint32_t getMaterialIndex();

			glm::vec2 getExtent(char* chr);
		};
	}
}