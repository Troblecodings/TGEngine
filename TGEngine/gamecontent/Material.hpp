#pragma once

#include "../Stdbase.hpp"
#include "../io/Files.hpp"

namespace tge::gmc {

	using namespace tge::nio;

	struct Material {
		glm::vec4 color;
		uint32_t  diffuseTexture;
	};

	extern Material* createdMaterials;

	void loadMaterials(File file, ResourceDescriptor* desc, uint32_t size);
}