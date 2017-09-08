#pragma once

#include "stdbase.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

namespace nio {

	using namespace std;

	SINCE(0, 0, 7)
	enum TagType
	{
		BOOLEAN, INT, FLOAT, STRING, NaN
	};

	SINCE(0, 0, 7)
	struct Tag
	{
		TagType type = STRING;
		char* name;
		char* value;
		Tag() {};
		Tag(char* iname, char* ivalue);
	};

	SINCE(0, 0, 7)
	struct BooleanTag : Tag
	{
		bool rvalue;
		BooleanTag() {};
		BooleanTag(char* iname, char* ivalue);
	};

	SINCE(0, 0, 7)
	struct IntTag : Tag
	{
		int rvalue;
		IntTag() {}
		IntTag(char* iname, char* ivalue);
	};

	SINCE(0, 0, 7)
	struct FloatTag : Tag
	{
		float rvalue;
		FloatTag() {};
		FloatTag(char* iname, char* ivalue);
	};

	SINCE(0, 0, 7)
	struct Properties
	{
		vector<Tag> strings;
		vector<BooleanTag> bools;
		vector<IntTag> ints;
		vector<FloatTag> floats;

		Tag getString(char* name);
		BooleanTag getBoolean(char* name);
		FloatTag getFloat(char* name);
		IntTag getInt(char* name);
	};

	/*
	* Reads a Property File in XML-Style
	*  Example XML:
	*  <xml>
	*    <boolean name="testboolean" value="true"/>
	*    <string name="teststring" value="test"/>
	*    <int name="testint" value="15"/>
	*    <float name="testfloat" value="5.4"/>
	*  </xml>
	*/
	SINCE(0, 0, 7)
	void readProperties(char* path, Properties* prop);
}