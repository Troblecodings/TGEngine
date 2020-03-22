#include "Actor.hpp"
#include "../pipeline/Descriptors.hpp"
#include "../TGEngine.hpp"

namespace tge::gmc {

	std::vector<ActorProperties> actorProperties;
	std::vector<ActorDescriptor> actorDescriptor;

	void loadToCommandBuffer(VkCommandBuffer buffer, uint8_t layerId) {

		for (size_t i = 0; i < actorProperties.size(); i++) {
			ActorProperties properties = actorProperties[i];
			if (properties.layer != layerId) continue;

			Material mat = createdMaterials[properties.material];
			vkCmdPushConstants(buffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, 64, &properties.localTransform);

			vkCmdPushConstants(buffer, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 64, 24, &mat);

			ActorDescriptor description = actorDescriptor[i];
			vkCmdDrawIndexed(buffer, description.indexDrawCount, 1, description.indexOffset, description.vertexOffset, 0);
		}

	}
}
