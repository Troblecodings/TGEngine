#include "Actor.hpp"
#include "../pipeline/Descriptors.hpp"
#include "../TGEngine.hpp"

namespace tge::gmc {

	std::vector<ActorProperties> actorProperties;
	std::vector<ActorDescriptor> actorDescriptor;


	// Internal tranform struct
	struct __Transform {
		glm::mat4 mat;
		uint32_t animationIndex;
	};

	void loadToCommandBuffer(VkCommandBuffer buffer, uint8_t layerId) {

		for (size_t i = 0; i < actorProperties.size(); i++) {
			ActorProperties properties = actorProperties[i];
			if (properties.layer != layerId) continue;

			// TODO change the incoming datapattern
			__Transform transf = { prop.localTransform, createdMaterials[prop.material].animationID };

			vkCmdPushConstants(buffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, 68, &transf);

			vkCmdPushConstants(buffer, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 68, 20, &createdMaterials[prop.material]);

			ActorDescriptor description = actorDescriptor[i];
			vkCmdDrawIndexed(buffer, description.indexDrawCount, 1, description.indexOffset, description.vertexOffset, 0);
		}

	}
}
