#pragma once

#include <map>
#include <string>
#include "../util/Annotations.hpp"
#include "../Error.hpp"
#include "Files.hpp"
#include <stdio.h>
#include <iostream>

namespace prop {

	using namespace std;

	SINCE(0, 0, 1)
		enum TagType {
		BOOLEAN, INT, FLOAT, STRING, NaN
	};

	/*
	 * Property storage -> only for tiny bits of data
	 */
	SINCE(0, 0, 1)
		class Properties {
		public:
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

		private:
			//name vectors
			map<char*, char*> strings;
			map<char*, bool> bools;
			map<char*, int> ints;
			map<char*, float> floats;
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