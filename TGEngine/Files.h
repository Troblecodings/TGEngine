#pragma once

#include "stdbase.h"
#include <fstream>
#include <vector>
#include <streambuf>

namespace nio {

	enum TagType
	{
		BOOLEAN, INT, FLOAT, STRING, NaN
	};

	struct Tag
	{
		TagType type = STRING;
		char* name;
		char* value;
		Tag(char* iname, char* ivalue) : name(iname), value(ivalue){};
	};

	struct BooleanTag : Tag
	{
		TagType type = BOOLEAN;
		bool value;
		BooleanTag(char* iname, bool ivalue) : Tag(iname, nullptr), value(ivalue) {};
	};

	struct IntTag : Tag
	{
		TagType type = INT;
		int value;
		IntTag(char* iname, int ivalue) : Tag(iname, nullptr), value(ivalue) {};
	};

	struct FloatTag : Tag
	{
		TagType type = FLOAT;
		float value;
		FloatTag(char* iname, float ivalue) : Tag(iname, nullptr), value(ivalue) {};
	};

	/*
	 *  Parameter:
	 *      char* fila_path -> path to your file 
	 *  Returns:
	 *      char* -> binary of your file declareted be #file_path
	 */
	SINCE(0, 0, 6)
	vector<char> getBinarys(char* file_path);

	SINCE(0, 0, 6)
	UNFINISHED
	vector<Tag> readXML(char* path);
}