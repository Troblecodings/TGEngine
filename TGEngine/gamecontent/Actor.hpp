#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include "../Error.hpp"
#include "../util/VectorUtil.hpp"
#include "Material.hpp"

namespace tge::gmc {

	struct ActorProperties {
		glm::mat4 localTransform;
		uint8_t   material;
		uint8_t   layer;
	};

	struct ActorInputInfo {
		ActorProperties pProperties;
		uint32_t  indexCount;
		uint32_t* indices;
		uint16_t  vertexCount;
		uint8_t*  vertices;
	};

	extern std::vector<ActorProperties> properties;
	extern std::vector<uint32_t>  countData;
	extern std::vector<uint32_t>  offsetData;

	void loadModel(File pFile, ResourceDescriptor* pResourceDescriptors, uint32_t pSize);

	void createActor(ActorInputInfo* pInputInfo, uint32_t pSize);

	void loadToCommandBuffer(VkCommandBuffer pBuffer, uint8_t pLayerId);

}