#include "ObjLoader.hpp"

namespace obj {
	using namespace std;

	void load(char* file, VertexBuffer* vert) {
		OUT_LV_DEBUG("Load OBJ File")
		vector<Vertex> verticies = {};
		vector<vector<uint32_t>> index = {};
		ifstream stream(file, ios::binary | ios::ate);
		streampos strm_pos = stream.tellg();
		stream.seekg(0);

		bool read_data = false;
		ObjMode mode = ObjMode::NaN;
		string cmd = "";
		vector<char*> data = {};
		rsize_t size = 0;

		Materials mat = {};
		vector<string> mtl_names;
		uint32_t material_index = 0;

		bool wait_for = false;
		
		for (size_t i = 0; i < strm_pos; i++)
		{
			char input;
			stream.read(&input, 1);
			if(read_data) {
				if (input != '\n' && input != '\r' && input != ' ' && input != '/' && !wait_for) {
					cmd += input;
				}
				switch (input) {
				case '/':
					wait_for = true;
					break;
				case ' ':
					wait_for = false;
					size = data.size();
					data.resize(size + 1);
					data[size] = new char[cmd.size() + 1];
					strcpy(data[size], cmd.c_str());
					cmd = "";
					break;
				case '\r':
				case '\n':
					size = data.size();
					data.resize(size + 1);
					data[size] = new char[cmd.size() + 1];
					strcpy(data[size], cmd.c_str());
					cmd = "";
					switch (mode)
					{
					case obj::VERTEX:
						size = verticies.size();
						verticies[size].pos[0] = stof(data[0]);
						verticies[size].pos[1] = stof(data[1]);
						verticies[size].pos[2] = stof(data[2]);
						verticies[size].color[0] = mat.diffuse[material_index].r;
						verticies[size].color[1] = mat.diffuse[material_index].g;
						verticies[size].color[2] = mat.diffuse[material_index].b;
						verticies[size].color[3] = mat.diffuse[material_index].a;
						break;
					case obj::INDEX:
						size = index.size();
						index.resize(size + 1);
						index[size] = { stoul(data[0]),  stoul(data[1]),  stoul(data[2])};
						break;
					case obj::MTL_LOAD:
						size = mtl_names.size();
						mtl_names.resize(size + 1);
						mtl_names[size] = string(data[0]);
						loadMaterial(mtl_names[size], &mat);
						break;
					case obj::USE_MTL:
						material_index = mat.find(data[0]);
						break;
					case obj::COMMENT:
                    #ifdef DEBUG
						for (char* ch : data) {
							cout << ch;
						}
						cout << endl;
                    #endif
						break;
					case obj::UV:
						break;
					case obj::NORMAL:
						break;
					case obj::NaN:
						break;
					}
					mode = ObjMode::NaN;
					cmd = "";
					read_data = false;
					data.clear();
					break;
				}
			}else {
			switch (input) {
			case ' ':
				read_data = true;
				cmd = "";
				break;
			case 'v':
				mode = VERTEX;
				break;
			case 'f':
				mode = INDEX;
				break;
			case '#':
				mode = COMMENT;
				break;
			case 'u':
				cmd += "u";
			NEXT_CHAR_AT(1, 's', 'u')
				break;
			NEXT_CHAR_AT(2, 'e', 's')
				break;
			NEXT_CHAR_AT(3, 'm', 'e')
	            else {
				cmd = "m";
			    }
			    break;
			NEXT_CHAR_AT(4, 't', 'm')
				CHECK_CHAR(1, 't', 'm')
				break;
			NEXT_CHAR_AT(5, 'l', 't')
				CHECK_CHAR(2, 'l', 't')
				CHECK_CHAR(3, 'l', 'l')
				break;
			NEXT_CHAR_AT(4, 'i', 'l')
				break;
			NEXT_CHAR_AT(5, 'b', 'i')
				break;
			case 'n':
				if (mode == VERTEX)mode = ObjMode::NaN;
				break;
			}
			if (cmd.compare("mtllib") == 0) {
				mode = ObjMode::MTL_LOAD;
			}
			else if(cmd.compare("usemtl") == 0) {
				mode = ObjMode::USE_MTL;
			}
			}
		}
		size = 0;
		for each (std::vector<uint32_t> var in index)
		{
			for each (uint32_t var2 in var)
			{
				vert->add(verticies[var2 - 1]);
				size++;
			}
		}
	}
	
	void loadMaterial(string material, Materials* mat) {
		ifstream stream(material, ios::binary | ios::ate);
		streampos strm_pos = stream.tellg();
		stream.seekg(0);

		MtlMode mode = MtlMode::Null;

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