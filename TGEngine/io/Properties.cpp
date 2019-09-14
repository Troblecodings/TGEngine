#include "Properties.hpp"

namespace prop {

	using namespace std;
	using namespace tge::nio;

	void readProperties(char* path, Properties* prop) {
		File file = open(path, "rb");

		if(!file) return;

		TagType tagtype = NaN;

		char* namebuf;
		char* valuebuf;

		int stage = 0;

		string buffer = "";
		size_t size = 0;

		while(!feof(file)) {
			char c_char;
			fread(&c_char, sizeof(char), 1, file);

			if(c_char == '<') {
				stage = 1;
				continue;
			} else if(c_char == '>' || c_char == '/') {
				if(tagtype != NaN) {
					switch(tagtype) {
						case BOOLEAN: prop->addBoolean(namebuf, strcmp(valuebuf, "true") == 0);
							break;
						case INT: prop->addInt(namebuf, stoi(valuebuf));
							break;
						case FLOAT: prop->addFloat(namebuf, stof(valuebuf));
							break;
						case STRING: prop->addString(namebuf, valuebuf);
							break;
					}
					tagtype = NaN;
					stage = 0;
					continue;
				}
				stage = 0;
				buffer.clear();
				continue;
			} else if(c_char == ' ') {
				if(stage == 1) {
					if(buffer.compare("boolean") == 0) {
						tagtype = BOOLEAN;
					} else if(buffer.compare("int") == 0) {
						tagtype = INT;
					} else if(buffer.compare("float") == 0) {
						tagtype = FLOAT;
					} else if(buffer.compare("string") == 0) {
						tagtype = STRING;
					} else {
						tagtype = NaN;
					}
					stage = 2;
					buffer.clear();
				}
				if(stage != 5 && stage != 6)continue;
			} else if(c_char == '=') {
				if(stage == 2) {
					if(buffer.compare("name") == 0) {
						stage = 3;
					} else if(buffer.compare("value") == 0) {
						stage = 4;
					} else {
						tagtype = NaN;
					}
					buffer.clear();
				}
				continue;
			} else if(c_char == '"') {
				if(stage == 3) {
					stage = 5;
				} else if(stage == 4) {
					stage = 6;
				} else if(stage == 5) {
					namebuf = new char[buffer.size() + 1];
					strcpy_s(namebuf, buffer.size() + 1, buffer.c_str());
					stage = 2;
				} else if(stage == 6) {
					valuebuf = new char[buffer.size() + 1];
					strcpy_s(valuebuf, buffer.size() + 1, buffer.c_str());
					stage = 2;
				}
				buffer.clear();
				continue;
			}
			if(stage >= 1) {
				buffer += c_char;
			}
		}
	}

	char* Properties::getStringOrDefault(char* name, char* def) {
		auto it = this->strings.find(name);
		if (it != this->strings.end())
			return it->second;
		return def;
	}

	bool Properties::getBooleanOrDefault(char* name, bool def) {
		auto it = this->bools.find(name);
		if (it != this->bools.end())
			return it->second;
		return def;
	}

	float Properties::getFloatOrDefault(char* name, float def) {
		auto it = this->floats.find(name);
		if (it != this->floats.end())
			return it->second;
		return def;
	}

	int Properties::getIntOrDefault(char* name, int def) {
		auto it = this->ints.find(name);
		if (it != this->ints.end())
			return it->second;
		return def;
	}

	char* Properties::getString(char* name) {
		return this->getStringOrDefault(name, "");
	}

	bool Properties::getBoolean(char* name) {
		return this->getBooleanOrDefault(name, false);
	}

	float Properties::getFloat(char* name) {
		return this->getFloatOrDefault(name, 0.0);
	}

	int Properties::getInt(char* name) {
		return this->getIntOrDefault(name, 0);
	}

	void Properties::addString(char* name, char* value) {
		this->strings[name] = value;
	}

	void Properties::addBoolean(char* name, bool value) {
		this->bools[name] = value;
	}

	void Properties::addFloat(char* name, float value) {
		this->floats[name] = value;
	}

	void Properties::addInt(char* name, int value) {
		this->ints[name] = value;
	}
}