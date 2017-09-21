#include "Properties.hpp"

namespace nio {

	using namespace std;

	void readProperties(char* path, Properties* prop) {
		ifstream input(path, ios::binary);

		TagType tagtype = NaN;

		char* namebuf;
		char* valuebuf;

		int stage = 0;

		string buffer = "";

		while (!input.eof())
		{
			char args[1];
			input.read(args, 1);
			if (args[0] == '<') {
				stage = 1;
				continue;
			}
			else if (args[0] == '>' || args[0] == '/') {
				size_t siz = 0;
				if (tagtype != NaN) {
					switch (tagtype) {
					case BOOLEAN: {
						siz = prop->bools.size();
						prop->bools.resize(siz + 1);
						prop->bools[siz] = BooleanTag(namebuf, valuebuf);
					}
								  break;
					case INT: {
						siz = prop->ints.size();
						prop->ints.resize(siz + 1);
						prop->ints[siz] = IntTag(namebuf, valuebuf);
					}
							  break;
					case FLOAT: {
						siz = prop->floats.size();
						prop->floats.resize(siz + 1);
						prop->floats[siz] = FloatTag(namebuf, valuebuf);
					}
								break;
					case STRING: {
						siz = prop->strings.size();
						prop->strings.resize(siz + 1);
						prop->strings[siz] = Tag(namebuf, valuebuf);
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
			else if (args[0] == ' ') {
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
			else if (args[0] == '=') {
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
			else if (args[0] == '"') {
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
				buffer += args[0];
			}
		}
	}

	Tag::Tag(char * iname, char * ivalue)
	{
		this->name = iname;
		this->value = ivalue;
	}

	BooleanTag::BooleanTag(char * iname, char* ivalue) : Tag(iname, ivalue)
	{
		this->rvalue = string("true").compare(ivalue) == 0;
		this->type = BOOLEAN;
	}

	FloatTag::FloatTag(char* iname, char* ivalue) : Tag(iname, ivalue)
	{
		this->rvalue = atof(ivalue);
		this->type = FLOAT;
	}

	IntTag::IntTag(char* iname, char* ivalue) : Tag(iname, ivalue)
	{
		this->rvalue = atoi(ivalue);
		this->type = INT;
	}

	BooleanTag Properties::getBoolean(char* name) {
		for each (BooleanTag val in this->bools) {
			if (string(val.name).compare(name) == 0)return val;
		}
	}

	Tag Properties::getString(char* name) {
		for each (Tag val in this->strings) {
			if (string(val.name).compare(name) == 0)return val;
		}
	}

	IntTag Properties::getInt(char* name) {
		for each (IntTag val in this->ints) {
			if (string(val.name).compare(name) == 0)return val;
		}
	}

	FloatTag Properties::getFloat(char* name) {
		for each (FloatTag val in this->floats) {
			if (string(val.name).compare(name) == 0)return val;
		}
	}

}