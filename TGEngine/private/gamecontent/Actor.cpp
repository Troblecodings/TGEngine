#include "../../public/gamecontent/Actor.hpp"
#include "../../public/pipeline/Descriptors.hpp"
#include "../../public/TGEngine.hpp"

namespace tge::gmc {

	std::vector<ActorProperties> actorProperties;
	std::vector<ActorDescriptor> actorDescriptor;
	std::vector<ActorInstanceDescriptor> actorInstanceDescriptor;

	void loadToCommandBuffer(VkCommandBuffer buffer, uint8_t layerId) {

		for (size_t i = 0; i < actorProperties.size(); i++) {
			ActorProperties properties = actorProperties[i];
			if (properties.layer != layerId) continue;

			vkCmdPushConstants(buffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, 72, &properties.transform);

			vkCmdPushConstants(buffer, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 72, 20, &createdMaterials[properties.material]);

			ActorDescriptor description = actorDescriptor[i];
			if(description.instanceID == UINT32_MAX)
				vkCmdDrawIndexed(buffer, description.indexDrawCount, 1, description.indexOffset, description.vertexOffset, 0);
			else {
				ActorInstanceDescriptor instanceDescriptor = actorInstanceDescriptor[description.instanceID];
				vkCmdDrawIndexed(buffer, description.indexDrawCount, instanceDescriptor.instanceCount, description.indexOffset, description.vertexOffset, instanceDescriptor.instanceOffset);
			}

		}

	}
}
