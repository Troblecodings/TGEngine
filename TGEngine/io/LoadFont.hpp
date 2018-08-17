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

struct Font {
	INPUT
	char* path;
	OPT float height = 30;

	OUTPUT
	Texture texture = {};
	stbtt_bakedchar cdata[256];

	SINCE(0, 0, 3)
	void drawString(TGVertex vert, char* string, VertexBuffer* buffer, float multi = 0.0015);
};

SINCE(0, 0, 3)
void loadfont(Font* font);