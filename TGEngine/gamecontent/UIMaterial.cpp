#include "UIMaterial.hpp"
#include "..\pipeline\Pipe.hpp"

namespace tge {
	namespace gmc {

		UIMaterial::UIMaterial(tex::TextureOutput* texture, glm::vec4 color) : Material(texture == nullptr ? &UIColorPipe : &UITexturedPipe) {
			this->type = UI_MAT_TYPE;
			this->texture = texture;
			this->color = color;
		}

		void UIMaterial::createMaterial() {
			this->pipe->precreation();
			vlibDepthStencilState.depthTestEnable = VK_FALSE;
			vlibRasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
			this->pipelineIndex = createPipeline(this->pipe->layoutIndex);
			vlibRasterizationState.cullMode = VK_CULL_MODE_FRONT_BIT;
			vlibDepthStencilState.depthTestEnable = VK_TRUE;
			this->descriptorIndex = createDescriptorSet(this->pipe->layoutIndex);

			if(this->texture != nullptr) {
				//tex::textureDescriptor.descriptorset = this->descriptorIndex;
				//tex::textureDescriptor.binding = 0;
				//this->texture->updateDescriptor();
			}
		}

	}
}