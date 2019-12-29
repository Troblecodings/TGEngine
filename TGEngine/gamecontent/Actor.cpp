#include "Actor.hpp"
#include "../pipeline/Descriptors.hpp"
#include "../TGEngine.hpp"

namespace tge::gmc {

	std::vector<ActorProperties> properties;
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
		properties.reserve(pSize);
		countData.reserve(pSize);
		offsetData.reserve(pSize);
		for (size_t i = 0; i < pSize; i++) {
			ActorInputInfo input = pInputInfo[i];
			properties.push_back(input.pProperties);
			countData.push_back(input.indexCount);
			offsetData.push_back(indexBuffer.indexCount);
			vertexBuffer.addAll(input.vertices, sizeof(glm::vec4) * input.vertexCount, input.vertexCount);
			indexBuffer.addAll(input.indices, input.indexCount);
		}
	}

	void loadToCommandBuffer(VkCommandBuffer buffer, uint8_t layerId) {

		for (size_t i = 0; i < properties.size(); i++) {
			ActorProperties prop = properties[i];
			if (prop.layer != layerId) continue;

			vkCmdPushConstants(buffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, 64, &prop.localTransform);

			vkCmdPushConstants(buffer, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 64, 20, &createdMaterials[prop.material]);

			vkCmdDrawIndexed(buffer, countData[i], 1, 0, offsetData[i], 0);
		}

	}
}
