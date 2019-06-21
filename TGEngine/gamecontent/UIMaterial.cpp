#include "UIMaterial.hpp"

namespace tge {
	namespace tex {
		
		UIMaterial::UIMaterial(Texture* texture, glm::vec4 color) : Material(texture == nullptr ? &UIColorPipe:&UITexturedPipe)
		{
			this->type = UI;
			this->texture = texture;
			this->color = color;
		}

		void UIMaterial::createMaterial()
		{
			this->pipe->precreation();
			vlib_depth_stencil_state.depthTestEnable = VK_FALSE;
			vlib_rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
			this->pipelineIndex = createPipeline(this->pipe->layoutIndex);
			vlib_rasterization_state.cullMode = VK_CULL_MODE_FRONT_BIT;
			vlib_depth_stencil_state.depthTestEnable = VK_TRUE;
			this->descriptorIndex = createDescriptorSet(this->pipe->layoutIndex);

			if (this->texture != nullptr) {
				textureDescriptor.descriptorset = this->descriptorIndex;
				this->texture->vulkanTexture->updateDescriptor();
			}
		}

	}
}