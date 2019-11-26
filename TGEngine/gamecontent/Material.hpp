#pragma once

#include "../Stdbase.hpp"
#include "../io/Files.hpp"

namespace tge::gmc {

	using namespace tge::nio;

	struct Material {
		uint32_t  diffuseTexture;
		glm::vec4 color;
	};

	extern Material* createdMaterials;

	void loadMaterials(File file, ResourceDescriptor* desc, uint32_t size);
}