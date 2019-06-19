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
			this->pipelineIndex = createPipeline(this->pipe->layoutIndex);
			this->descriptorIndex = createDescriptorSet(this->pipe->layoutIndex);

			if (this->texture != nullptr) {
				textureDescriptor.descriptorset = this->descriptorIndex;
				this->texture->vulkanTexture->updateDescriptor();
			}
		}

	}
}