#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include "../Error.hpp"
#include "../util/VectorUtil.hpp"
#include "Material.hpp"

namespace tge::gmc {

	struct ActorInputInfo {
		glm::mat4 localTransform;
		uint32_t*  indices;
		uint8_t*  vertices;
		uint8_t   material;
		uint32_t  indexCount;
		uint16_t  vertexCount;
	};

	extern std::vector<glm::mat4> localTranformIds;
	extern std::vector<uint32_t> countData;
	extern std::vector<uint32_t> offsetData;
	extern std::vector<uint8_t>  materialIds;

	void loadModel(File pFile, ResourceDescriptor* pResourceDescriptors, uint32_t pSize);

	void createActor(ActorInputInfo* pInputInfo, uint32_t pSize);

	void loadToCommandBuffer(VkCommandBuffer pBuffer);

}