#include "Actor.hpp"
#include "../pipeline/Descriptors.hpp"

namespace tge::gmc {

	std::vector<uint8_t> localTranformIds;
	std::vector<uint8_t> materialIds;
	std::vector<std::vector<uint8_t>> actorData;

	void loadModel(File pFile, ResourceDescriptor* pResourceDescriptors, uint32_t pSize) {
		for (uint32_t i = 0; i < pSize; i++) {
			ResourceDescriptor descriptor = pResourceDescriptors[i];
				
			fseek(pFile, descriptor.offset, SEEK_SET);
			uint8_t* buffer = new uint8_t[pSize];
			fread(buffer, sizeof(uint8_t), descriptor.size, pFile);
		}
	}

	void loadToCommandBuffer(VkCommandBuffer buffer) {

		for (size_t i = 0; i < localTranformIds.size(); i++) {

			vkCmdPushConstants(buffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, 4, 0);

			vkCmdPushConstants(buffer, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 4, 4, 0);

		}

	}
}
