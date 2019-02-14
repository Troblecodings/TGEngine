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

struct Font {
	INPUT
	char* path;
	OPT float height = 30;

	OUTPUT
	Texture texture;
	stbtt_bakedchar cdata[256];

	SINCE(0, 0, 3)
	float drawString(glm::vec3 pos, glm::vec4 color, char* string, VertexBuffer* buffer, IndexBuffer* ibuffer, float multi = 0.0015);
};

SINCE(0, 0, 3)
void loadfont(Font* font);