#pragma once

#include "../Stdbase.hpp"
#include "../io/Files.hpp"

namespace tge::gmc {

	using namespace tge::nio;

	/*
	 * This holdes the materials for the render
	 * this is equivalent to the according push constante
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
	 * Loads the materials into the createdMaterials
	 * array deined by this file
	 */
	void loadMaterials(File file, ResourceDescriptor* desc, uint32_t size);
}
