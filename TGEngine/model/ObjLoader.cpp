#include "ObjLoader.hpp"

namespace obj {

	using namespace std;
	using namespace nio;

	void load(char* name, VertexBuffer* vert) {
		long size = 0;
		File file = readFileSize(name, "rb", &size);

		vector<TGVertex> vertecies = {};

		string buffer;
		Mode mode = NONE;

		for (long i = 0; i < size; i++)
		{
			char character;
			fread(&character, sizeof(char), 1, file);

			if (character == ' ') {
				SET_MODE("v", VERTEX)
				else SET_MODE("vt", UV)
				else SET_MODE("vn", NORMAL)
				else SET_MODE("f", FACE)
				else SET_MODE("mtllib", MTL_LOAD)
				else SET_MODE("usemtl", USE_MTL)
				else SET_MODE("s", SMOTHING);
			    else if (character == '\n' || character == '\r' ) {
					mode = NONE;
				}
				buffer.clear();
			}
			else {
				buffer.append(&character);
			}
		}
	}
	
	void loadMaterial(string material, Materials* mat) {
		ifstream stream(material, ios::binary | ios::ate);
		streampos strm_pos = stream.tellg();
		stream.seekg(0);

		Mode mode = NONE;

		vector<char*> data = {};
		string cmd = "";
		bool read_data = false;
		rsize_t size = 0;
		uint32_t current;

		for (size_t i = 0; i < strm_pos; i++)
		{
			char input;
			stream.read(&input, 1);
			if (read_data) {
				if (input != ' ' && input != '\n' && input != '\r') {
					cmd += input;
				}
				switch (input)
				{
				case ' ':
					size = data.size();
					data.resize(size + 1);
					data[size] = new char[cmd.size() + 1];
					strcpy(data[size], cmd.c_str());
					cmd = "";
					break;
				case '\n':
				case '\r':
					size = data.size();
					data.resize(size + 1);
					data[size] = new char[cmd.size() + 1];
					strcpy(data[size], cmd.c_str());
					read_data = false,
					cmd = "";
					switch (mode) {
					case MtlMode::NEW:
						current = mat->add(data[0]);
						break;
					case MtlMode::DIFFUSE:
						mat->diffuse[current] = { stof(data[0]), stof(data[1]), stof(data[2]), 1};
						break;
					}
					mode = MtlMode::Null;
					data.clear();
					break;
				}
			}
			else {
				if (input == ' ') {
					if (cmd.compare("newmtl") == 0) {
						mode = MtlMode::NEW;
						
						cmd = "";
					}
					else if (cmd.compare("Kd") == 0) {
						mode = MtlMode::DIFFUSE;
						cmd = "";
					}
					read_data = true;
				}
				else if(input != '\n' && input != '\r') {
					cmd += input;
				}
			}
		}
	}

	uint32_t obj::Materials::add(char * name)
	{
		uint32_t size = this->names.size();
		this->names.resize(size + 1);
		this->diffuse.resize(size + 1);

		this->names[size] = name;
		return size;
	}

	uint32_t obj::Materials::find(char * name)
	{
		string st(name);
		uint32_t retrn = 0;
		for each(char* ch in this->names) {
			if (st.compare(ch) == 0) {
				return retrn;
			}
			retrn++;
		}
		return retrn;
	}
}