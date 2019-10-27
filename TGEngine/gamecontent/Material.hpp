#pragma once

#include "../Stdbase.hpp"
#include "../io/Files.hpp"

namespace tge::gmc {

	using namespace tge::nio;

	struct Material {
		uint32_t diffuseTexture;
		glm::vec4 color;
		glm::vec2 uvmanipulator;
		uint32_t id; 
	};

	extern VkBuffer materialUBO;
	extern VkDeviceMemory materialMemory;

	void inline initMaterials();

	void inline loadMaterials(File file, ResourceDescriptor* desc, uint32_t size, Material* material);

	void inline createMaterials(Material* material, uint32_t size);
}