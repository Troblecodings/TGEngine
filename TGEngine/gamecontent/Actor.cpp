#include "Actor.hpp"
#include "../pipeline/Descriptors.hpp"
#include "../TGEngine.hpp"

namespace tge::gmc {

	std::vector<glm::mat4> localTranformIds;
	std::vector<uint32_t>  countData;
	std::vector<uint32_t>  offsetData;
	std::vector<uint8_t>  materialIds;

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
		localTranformIds.reserve(pSize);
		countData.reserve(pSize);
		offsetData.reserve(pSize);
		materialIds.reserve(pSize);
		for (size_t i = 0; i < pSize; i++) {
			ActorInputInfo input = pInputInfo[i];
			localTranformIds.push_back(input.localTransform);
			countData.push_back(input.indexCount);
			offsetData.push_back(indexBuffer.indexCount);
			materialIds.push_back(input.material);
			vertexBuffer.addAll(input.vertices, sizeof(glm::vec4) * input.vertexCount, input.vertexCount);
			indexBuffer.addAll(input.indices, input.indexCount);
		}
	}

	void loadToCommandBuffer(VkCommandBuffer buffer) {

		for (size_t i = 0; i < localTranformIds.size(); i++) {
			vkCmdPushConstants(buffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, 64, &localTranformIds[i]);

			vkCmdPushConstants(buffer, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 64, 20, &createdMaterials[materialIds[i]]);

			vkCmdDrawIndexed(buffer, countData[i], 1, 0, offsetData[i], 0);
		}

	}
}
