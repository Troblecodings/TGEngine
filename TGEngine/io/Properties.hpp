#pragma once

#include <map>
#include <string>
#include "../Error.hpp"
#include "Files.hpp"
#include <stdio.h>
#include <iostream>

namespace prop {

	using namespace std;

	enum TagType {
		BOOLEAN, INT, FLOAT, STRING, NaN
	};

	/*
	 * Property storage -> only for tiny bits of data
	 */
	class Properties {
		public:
			/*
			 * Gets the value or returns 0. false or an empty string
			 */
				char* getString(const char* name);
			bool getBoolean(const char* name);
			float getFloat(const char* name);
			int getInt(const char* name);

			/*
			 * Gets the value or returns the default if it shouldn't find the name
			 */
				char* getStringOrDefault(const char* name, char* def);
			bool getBooleanOrDefault(const char* name, bool def);
			float getFloatOrDefault(const char* name, float def);
			int getIntOrDefault(const char* name, int def);

			/*
			 * Add values to the properties
			 */
				void addString(const char* name, char* value);
			void addBoolean(const char* name, bool value);
			void addFloat(const char* name, float value);
			void addInt(const char* name, int value);

		private:
			//name vectors
			map<std::string, char*> strings;
			map<std::string, bool> bools;
			map<std::string, int> ints;
			map<std::string, float> floats;
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