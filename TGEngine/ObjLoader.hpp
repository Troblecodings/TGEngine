#pragma once

#include "TGEngine.hpp"
#include "Vertex.hpp"
#include <fstream>

namespace obj {

#define CHECK_CHAR(index, ure_char, char_befor)if (cmd.size() == index && cmd.at(index - 1) == char_befor) {\
		cmd += ure_char;\
        break;\
	}
#define NEXT_CHAR_AT(index, ure_char, char_befor) case ure_char:\
	CHECK_CHAR(index, ure_char, char_befor)


	SINCE(0, 0, 2)
	enum ObjMode {
		NaN,
		VERTEX,
		INDEX,
		MTL_LOAD,
		USE_MTL,
		COMMENT,

		UNFINISHED
		UV,
		NORMAL
		END_UNFINISHED
	};

	SINCE(0, 0, 2)
	enum MtlMode {
		Null,
		NEW,
		DIFFUSE
	};

	struct Materials {
		std::vector<char*> names;
		std::vector<glm::vec3> diffuse;

		uint32_t add(char* name);
		uint32_t find(char* name);
	};

	SINCE(0, 0, 2)
	void load(char* file, std::vector<Vertex>* verticies);

	SINCE(0, 0, 2)
	void loadMaterial(std::string material, Materials* mat);

}