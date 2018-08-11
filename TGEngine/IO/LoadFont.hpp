#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include "..\Util\Debug.hpp"
#include "FontTables.hpp"
#include "../Pipeline/Buffer/Texturebuffer.hpp"
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include "..\stb\stb_truetype.h"
#include "../Drawlib/Rectangle.hpp"

#define MULTIPLAIER * 0.003

struct Font {
	INPUT
	char* path;

	OUTPUT
	Texture texture = {};
	stbtt_bakedchar cdata[256];

	void drawString(TGVertex vert, char* string, VertexBuffer* buffer);
};

SINCE(0, 0, 3)
void loadfont(Font* font);