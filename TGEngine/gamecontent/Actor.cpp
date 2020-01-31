#include "Actor.hpp"
#include "../pipeline/Descriptors.hpp"
#include "../TGEngine.hpp"

namespace tge::gmc {

	std::vector<ActorProperties> properties;
	std::vector<uint32_t>  countData;
	std::vector<uint32_t>  offsetData;
	std::vector<uint32_t>  vertexOffsetData;

	void loadModel(File pFile, ResourceDescriptor* pResourceDescriptors, uint32_t pSize) {
		for (uint32_t i = 0; i < pSize; i++) {
			ResourceDescriptor descriptor = pResourceDescriptors[i];

			fseek(pFile, (long)descriptor.offset, SEEK_SET);
			uint8_t* buffer = new uint8_t[pSize];
			fread(buffer, sizeof(uint8_t), descriptor.size, pFile);
			// TODO Deserilize
		}
	}

	void createActor(ActorInputInfo* pInputInfo, uint32_t pSize) {
		properties.reserve(pSize);
		countData.reserve(pSize);
		offsetData.reserve(pSize);
		vertexOffsetData.reserve(pSize);

		for (size_t i = 0; i < pSize; i++) {
			ActorInputInfo input = pInputInfo[i];
			properties.push_back(input.pProperties);
			countData.push_back(input.indexCount);
			offsetData.push_back(indexBuffer.indexCount);
			indexBuffer.addAll(input.pIndices, input.indexCount);
			vertexOffsetData.push_back((uint32_t)vertexBuffer.pointCount);
			vertexBuffer.addAll(input.pVertices, sizeof(glm::vec4) * input.vertexCount, input.vertexCount);
		}
	}

	void loadToCommandBuffer(VkCommandBuffer buffer, uint8_t layerId) {

		for (size_t i = 0; i < properties.size(); i++) {
			ActorProperties prop = properties[i];
			if (prop.layer != layerId) continue;

			vkCmdPushConstants(buffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, 64, &prop.localTransform);

			vkCmdPushConstants(buffer, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 64, 20, &createdMaterials[prop.material]);

			vkCmdDrawIndexed(buffer, countData[i], 1, offsetData[i], vertexOffsetData[i], 0);
		}

	}
}
