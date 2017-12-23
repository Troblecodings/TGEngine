#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include "Debug.hpp"
#include "FontTables.hpp"

#define SHIFT_ADD(chr, chr_array) if(strlen(chr_array) < 4){ chr_array[strlen(chr_array)] = chr; }\
else { chr_array[0] = chr_array[1];chr_array[1] = chr_array[2];chr_array[2] = chr_array[3];chr_array[3] = chr; }

struct FontTableEntry {
	std::vector<char*> name_entry;
	std::vector<BaseTable> content;

	uint32_t createIfNotExists(char* name);
};

void loadfont(char* path);

char* getName(FontTable table);