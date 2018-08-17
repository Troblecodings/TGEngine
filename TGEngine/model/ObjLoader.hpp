#pragma once

#include "..\Stdbase.hpp"
#include "..\Pipeline\Buffer\VertexBuffer.hpp"
#include "..\io\Files.hpp"

namespace obj {

#define SET_MODE(ch, mod) if (buffer.compare(ch) == 0) {\
mode = mod;\
}


	SINCE(0, 0, 2)
	enum Mode {
		NONE,
		VERTEX,
		INDEX,
		MTL_LOAD,
		USE_MTL,
		COMMENT,
		UV,
		SMOTHING,
		NORMAL,
		FACE,

		NEW_MATERIAL,
		DIFFUSE,
		AMBIENT,
		SPECULAR,
		ALPHA,
		ILLUMINATION,
		ROUGHNESS,
		METALLIC,
		SHEEN,
		CLEARCOAT_THICKNESS,
		CLEARCOAT_ROUGHNESS,
		EMISSIVE,
		ANISOTROPY,
		ANISOTROPY_ROTATION,
		MAP_ROUGHNESS,
		MAP_METALLIC,
		MAP_SHEEN,
		MAP_EMISSIVE,
		MAP_DIFFUSE,
		MAP_AMBIENT,
		MAP_SPECULAR,
		MAP_ALPHA,
		MAP_BUMP,
		MAP_DISPLACEMENT,
		MAP_STENCIL_DECAL,
		MAP_REFLACTION,
		MAP_NORMAL
	};

	struct Materials {
		std::vector<char*> names;
		std::vector<glm::vec3> diffuse;

		uint32_t add(char* name);
		uint32_t find(char* name);
	};

	SINCE(0, 0, 2)
	void load(char* file, VertexBuffer* verticies);

	SINCE(0, 0, 2)
	void loadMaterial(std::string material, Materials* mat);

}