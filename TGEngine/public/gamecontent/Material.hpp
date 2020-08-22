#pragma once

#include "../Stdbase.hpp"
#include "../io/Files.hpp"

namespace tge::gmc {

	using namespace tge::nio;

	constexpr uint32_t MAX_MATERIALS = 256;

	/*
	 * Holds the materials for the render,
	 * equivalent to the according push constant
	 * (controls animation)
	 */
	struct Material {
		glm::vec4 color;
		uint32_t  diffuseTexture;
	};

	/* Holds all the materials for the push constant
	 * <em>Note: Needs to be initialized before a load call</em>
	 */
	extern Material* createdMaterials;
}
