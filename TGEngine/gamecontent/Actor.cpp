#include "Actor.hpp"
#include "../pipeline/Descriptors.hpp"

namespace tge::gmc {

	std::vector<glm::mat4> localTranformIds;
	std::vector<uint32_t>  countData;
	std::vector<uint32_t>  offsetData;

	void loadModel(File pFile, ResourceDescriptor* pResourceDescriptors, uint32_t pSize) {
		for (uint32_t i = 0; i < pSize; i++) {
			ResourceDescriptor descriptor = pResourceDescriptors[i];
				
			fseek(pFile, descriptor.offset, SEEK_SET);
			uint8_t* buffer = new uint8_t[pSize];
			fread(buffer, sizeof(uint8_t), descriptor.size, pFile);
			// TODO Deserilize
		}
	}

	void createActor(ActorInputInfo* pInputInfo, uint32_t pSize) {
		localTranformIds.resize(pSize);
		countData.resize(pSize);
		offsetData.resize(pSize);
		for (size_t i = 0; i < pSize; i++) {
			localTranformIds.push_back(pInputInfo[i].localTransform);
			countData.push_back(pInputInfo[i].size);
			offsetData.push_back(pInputInfo[i].offset);
		}
	}

	void loadToCommandBuffer(VkCommandBuffer buffer) {

		for (size_t i = 0; i < localTranformIds.size(); i++) {
			vkCmdPushConstants(buffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, 64, &localTranformIds[i]);

			vkCmdPushConstants(buffer, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 64, 20, &createdMaterials[i]);

			vkCmdDrawIndexed(buffer, countData[i], 1, 0, offsetData[i], 0);
		}

	}
}
