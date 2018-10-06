#include "Properties.hpp"

namespace prop {

	using namespace std;
	using namespace nio;

	void readProperties(char* path, Properties* prop) {
		File file = open(path, "rb");

		TagType tagtype = NaN;

		char* namebuf;
		char* valuebuf;

		int stage = 0;

		string buffer = "";
		size_t size = 0;

		while (!feof(file))
		{
			char c_char;
			fread(&c_char, sizeof(char), 1, file);

			if (c_char == '<') {
				stage = 1;
				continue;
			}
			else if (c_char == '>' || c_char == '/') {
				if (tagtype != NaN) {
					switch (tagtype) {
					case BOOLEAN: {
						size = prop->bool_names.size();
						prop->bool_names.resize(size + 1);
						prop->bool_values.resize(size + 1);
						prop->bool_names[size] = namebuf;
						prop->bool_values[size] = strcmp(valuebuf, "true") == 0;
					}
								  break;
					case INT: {
						size = prop->int_names.size();
						prop->int_names.resize(size + 1);
						prop->int_values.resize(size + 1);
						prop->int_names[size] = namebuf;
						prop->int_values[size] = stoi(valuebuf);
					}
							  break;
					case FLOAT: {
						size = prop->float_names.size();
						prop->float_names.resize(size + 1);
						prop->float_values.resize(size + 1);
						prop->float_names[size] = namebuf;
						prop->float_values[size] = stof(valuebuf);
					}
								break;
					case STRING: {
						size = prop->bool_names.size();
						prop->string_names.resize(size + 1);
						prop->string_values.resize(size + 1);
						prop->string_names[size] = namebuf;
						prop->string_values[size] = valuebuf;
					}
								 break;
					}
					tagtype = NaN;
					stage = 0;
					continue;
				}
				stage = 0;
				buffer.clear();
				continue;
			}
			else if (c_char == ' ') {
				if (stage == 1) {
					if (buffer.compare("boolean") == 0) {
						tagtype = BOOLEAN;
					}
					else if (buffer.compare("int") == 0) {
						tagtype = INT;
					}
					else if (buffer.compare("float") == 0) {
						tagtype = FLOAT;
					}
					else if (buffer.compare("string") == 0) {
						tagtype = STRING;
					}
					else {
						tagtype = NaN;
					}
					stage = 2;
					buffer.clear();
				}
				if (stage != 5 && stage != 6)continue;
			}
			else if (c_char == '=') {
				if (stage == 2) {
					if (buffer.compare("name") == 0) {
						stage = 3;
					}
					else if (buffer.compare("value") == 0) {
						stage = 4;
					}
					else {
						tagtype = NaN;
					}
					buffer.clear();
				}
				continue;
			}
			else if (c_char == '"') {
				if (stage == 3) {
					stage = 5;
				}
				else if (stage == 4) {
					stage = 6;
				}
				else if (stage == 5) {
					namebuf = new char[buffer.size() + 1];
					strcpy(namebuf, buffer.c_str());
					stage = 2;
				}
				else if (stage == 6) {
					valuebuf = new char[buffer.size() + 1];
					strcpy(valuebuf, buffer.c_str());
					stage = 2;
				}
				buffer.clear();
				continue;
			}
			if (stage >= 1) {
				buffer += c_char;
			}
		}
	}

	char* Properties::getString(char* name) {
		for (size_t i = 0; i < this->string_names.size(); i++)
		{
			if (strcmp(name, this->string_names[i]) == 0) {
				return this->string_values[i];
			}
		}
		return "";
	}

	bool Properties::getBoolean(char* name) {
		for (size_t i = 0; i < this->bool_names.size(); i++)
		{
			if (strcmp(name, this->bool_names[i]) == 0) {
				return this->bool_values[i];
			}
		}
		return false;
	}

	float Properties::getFloat(char* name) {
		for (size_t i = 0; i < this->float_names.size(); i++)
		{
			if (strcmp(name, this->float_names[i]) == 0) {
				return this->float_values[i];
			}
		}
		return 0;
	}

	int Properties::getInt(char* name) {
		for (size_t i = 0; i < this->int_names.size(); i++)
		{
			if (strcmp(name, this->int_names[i]) == 0) {
				return this->int_values[i];
			}
		}
		return 0;
	}

}