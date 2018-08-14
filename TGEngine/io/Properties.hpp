#pragma once

#include <vector>
#include <string>
#include "..\util\Annotations.hpp"
#include <stdio.h>
#include <iostream>

namespace nio {

	using namespace std;

	SINCE(0, 0, 1)
	enum TagType
	{
		BOOLEAN, INT, FLOAT, STRING, NaN
	};

	SINCE(0, 0, 1)
	struct Properties
	{
		//name vectors
		vector<char*> string_names;
		vector<char*> bool_names;
		vector<char*> int_names;
		vector<char*> float_names;
		//value vectors
		vector<char*> string_values;
		vector<bool> bool_values;
		vector<int> int_values;
		vector<float> float_values;

		char* getString(char* name);
		bool getBoolean(char* name);
		float getFloat(char* name);
		int getInt(char* name);
	};

	/*
	* Reads a Property File in XML-Style
	*  Example XML:
	*  <xml>
	*    <boolean name="testboolean" value="true"/>
	*    <string name="teststring" value="test"/>
	*    <int name="testint" value="117"/>
	*    <float name="testfloat" value="0.9"/>
	*  </xml>
	*/
	SINCE(0, 0, 1)
	void readProperties(char* path, Properties* prop);
}