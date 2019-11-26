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
		uint32_t offset;
		uint32_t size;
	};

	extern std::vector<glm::mat4> localTranformIds;
	extern std::vector<uint32_t> countData;
	extern std::vector<uint32_t> offsetData;

	void loadModel(File pFile, ResourceDescriptor* pResourceDescriptors, uint32_t pSize);

	void createActor(ActorInputInfo* pInputInfo, uint32_t pSize);

	void loadToCommandBuffer(VkCommandBuffer pBuffer);

}