#include "../../public/io/Properties.hpp"

namespace tge::pro {

	using namespace std;
	using namespace tge::nio;

	Properties readProperties(char* path) {
		Properties rtProperties;
		File file = open(path, "rb");

		if (!file) return rtProperties;

		TagType tagtype = NaN;

		char* namebuf;
		char* valuebuf;

		int stage = 0;

		string buffer = "";
		size_t size = 0;

		while (!feof(file)) {
			char c_char;
			fread(&c_char, sizeof(char), 1, file);

			if (c_char == '<') {
				stage = 1;
				continue;
			} else if (c_char == '>' || c_char == '/') {
				if (tagtype != NaN) {
					switch (tagtype) {
					case BOOLEAN: rtProperties.addBoolean(namebuf, strcmp(valuebuf, "true") == 0);
						break;
					case INT: rtProperties.addInt(namebuf, stoi(valuebuf));
						break;
					case FLOAT: rtProperties.addFloat(namebuf, stof(valuebuf));
						break;
					case STRING: rtProperties.addString(namebuf, valuebuf);
						break;
					}
					tagtype = NaN;
					stage = 0;
					continue;
				}
				stage = 0;
				buffer.clear();
				continue;
			} else if (c_char == ' ') {
				if (stage == 1) {
					if (buffer.compare("boolean") == 0) {
						tagtype = BOOLEAN;
					} else if (buffer.compare("int") == 0) {
						tagtype = INT;
					} else if (buffer.compare("float") == 0) {
						tagtype = FLOAT;
					} else if (buffer.compare("string") == 0) {
						tagtype = STRING;
					} else {
						tagtype = NaN;
					}
					stage = 2;
					buffer.clear();
				}
				if (stage != 5 && stage != 6)continue;
			} else if (c_char == '=') {
				if (stage == 2) {
					if (buffer.compare("name") == 0) {
						stage = 3;
					} else if (buffer.compare("value") == 0) {
						stage = 4;
					} else {
						tagtype = NaN;
					}
					buffer.clear();
				}
				continue;
			} else if (c_char == '"') {
				if (stage == 3) {
					stage = 5;
				} else if (stage == 4) {
					stage = 6;
				} else if (stage == 5) {
					namebuf = new char[buffer.size() + 1];
					strcpy_s(namebuf, buffer.size() + 1, buffer.c_str());
					stage = 2;
				} else if (stage == 6) {
					valuebuf = new char[buffer.size() + 1];
					strcpy_s(valuebuf, buffer.size() + 1, buffer.c_str());
					stage = 2;
				}
				buffer.clear();
				continue;
			}
			if (stage >= 1) {
				buffer += c_char;
			}
		}
		return rtProperties;
	}

	char* Properties::getStringOrDefault(const char* name, char* def) {
		for (size_t i = 0; i < this->stringNames.size(); i++)
			if (strcmp(this->stringNames[i], name) == 0)
				return this->strings[i];
		return def;
	}

	bool Properties::getBooleanOrDefault(const char* name, bool def) {
		for (size_t i = 0; i < this->boolNames.size(); i++)
			if (strcmp(this->boolNames[i], name) == 0)
				return this->bools[i];
		return def;
	}

	float Properties::getFloatOrDefault(const char* name, float def) {
		for (size_t i = 0; i < this->floatNames.size(); i++)
			if (strcmp(this->floatNames[i], name) == 0)
				return this->floats[i];
		return def;
	}

	int Properties::getIntOrDefault(const char* name, int def) {
		for (size_t i = 0; i < this->intNames.size(); i++)
			if (strcmp(this->intNames[i], name) == 0)
				return this->ints[i];
		return def;
	}

	char* Properties::getString(const char* name) {
		return this->getStringOrDefault(name, "");
	}

	bool Properties::getBoolean(const char* name) {
		return this->getBooleanOrDefault(name, false);
	}

	float Properties::getFloat(const char* name) {
		return this->getFloatOrDefault(name, 0.0);
	}

	int Properties::getInt(const char* name) {
		return this->getIntOrDefault(name, 0);
	}

	void Properties::addString(const char* name, char* value) {
		this->stringNames.push_back(name);
		this->strings.push_back(value);
	}

	void Properties::addBoolean(const char* name, bool value) {
		this->boolNames.push_back(name);
		this->bools.push_back(value);
	}

	void Properties::addFloat(const char* name, float value) {
		this->floatNames.push_back(name);
		this->floats.push_back(value);
	}

	void Properties::addInt(const char* name, int value) {
		this->intNames.push_back(name);
		this->ints.push_back(value);
	}
}