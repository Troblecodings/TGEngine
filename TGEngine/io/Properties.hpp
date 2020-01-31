#pragma once

#include <map>
#include <string>
#include "../Error.hpp"
#include "Files.hpp"
#include <stdio.h>
#include <iostream>

namespace tge::pro {

	enum TagType {
		BOOLEAN, INT, FLOAT, STRING, NaN
	};

	/*
	 * Property storage -> only for tiny bits of data
	 */
	struct Properties {
		//name vectors
		std::vector<char*> strings;
		std::vector<bool>  bools;
		std::vector<int>   ints;
		std::vector<float> floats;

		std::vector<const char*> stringNames;
		std::vector<const char*> boolNames;
		std::vector<const char*> intNames;
		std::vector<const char*> floatNames;

		/*
		 * Gets the value or returns 0, false or an empty string
		 */
		char* getString(const char* name);
		bool  getBoolean(const char* name);
		float getFloat(const char* name);
		int   getInt(const char* name);

		/*
		 * Gets the value or returns the default in case it can't find the name
		 */
		char* getStringOrDefault(const char* name, char* def);
		bool  getBooleanOrDefault(const char* name, bool def);
		float getFloatOrDefault(const char* name, float def);
		int   getIntOrDefault(const char* name, int def);

		/*
		 * Add values to the tgeproperties
		 */
		void addString(const char* name, char* value);
		void addBoolean(const char* name, bool value);
		void addFloat(const char* name, float value);
		void addInt(const char* name, int value);
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
	Properties readProperties(char* path);
}
