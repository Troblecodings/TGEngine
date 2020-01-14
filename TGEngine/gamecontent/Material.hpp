#pragma once

#include "../Stdbase.hpp"
#include "../io/Files.hpp"

namespace tge::gmc {

	using namespace tge::nio;

	/*
	 * Holds the materials for the render,
	 * equivalent to the according push constant
	 */
	struct Material {
		glm::vec4 color;
		uint32_t  diffuseTexture;
	};

	/* Holds all the materials for the push constant
	 * <em>Note: This needs to be initalized before a load call</em>
	 */
	extern Material* createdMaterials;

	/*
	 * Loads all materials into the createdMaterials
	 * that are defined in the given file
	 */
	void loadMaterials(File file, ResourceDescriptor* desc, uint32_t size);
}
