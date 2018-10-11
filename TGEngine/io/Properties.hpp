#pragma once

#include <vector>
#include <string>
#include "..\util\Annotations.hpp"
#include "..\Error.h"
#include "Files.hpp"
#include <stdio.h>
#include <iostream>

namespace prop {

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

		/*
         * Gets the value or returns 0. false or an empty string
		 */
		SINCE(0, 0, 3)
		char* getString(char* name);
		bool getBoolean(char* name);
		float getFloat(char* name);
		int getInt(char* name);

		/*
         * Gets the value or returns the default if it shouldn't find the name
         */
		SINCE(0, 0, 4)
		char* getStringOrDefault(char* name, char* def);
		bool getBooleanOrDefault(char* name, bool def);
		float getFloatOrDefault(char* name, float def);
		int getIntOrDefault(char* name, int def);

		/*
		 * Add values to the properties
		 */
		SINCE(0, 0, 4)
		void addString(char* name, char* value);
		void addBoolean(char* name, bool value);
		void addFloat(char* name, float value);
		void addInt(char* name, int value);
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