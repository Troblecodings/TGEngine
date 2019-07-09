#include "Material.hpp"
#include "Light.hpp"

namespace tge {
	namespace gmc {

		std::vector<Material*> materiallist;
		std::vector<RenderOffsets> render_offset;

		Material::Material(tex::Texture* texture, glm::vec4 color) {
			this->texture = texture;
			this->color = color;

			VkSpecializationMapEntry* map_entrys = new VkSpecializationMapEntry[4];
			for(uint32_t i = 0; i < 4; i++) {
				map_entrys[i].constantID = i;
				map_entrys[i].offset = i * sizeof(float);
				map_entrys[i].size = sizeof(float);
			}

			VkSpecializationInfo pSpecialization;
			pSpecialization.mapEntryCount = 4;
			pSpecialization.pData = &this->color;
			pSpecialization.dataSize = sizeof(float) * 4;
			pSpecialization.pMapEntries = map_entrys;
			VertexTextured.pSpecializationInfo = &pSpecialization;

			if(texture == nullptr) {
				this->pipe = &TexturedBasicPipe;
			} else {
				this->pipe = &TexturedBasicPipe;
			}
		}

		Material::Material(ShaderPipe* pipe) {
			this->pipe = pipe;
		}

		void Material::createMaterial() {
			this->pipe->precreation();
			this->pipelineIndex = createPipeline(this->pipe->layoutIndex);

			this->descriptorIndex = createDescriptorSet(this->pipe->layoutIndex);
			camera_uniform.descriptor.descriptorset = lightbuffer.descriptor.descriptorset = this->descriptorIndex;

			camera_uniform.descriptor.binding = 0;
			camera_uniform.updateDescriptor();

			lightbuffer.descriptor.binding = 1;
			lightbuffer.updateDescriptor();

			if(this->texture != nullptr) {
				tex::textureDescriptor.descriptorset = this->descriptorIndex;
				tex::textureDescriptor.binding = 2;
				this->texture->vulkanTexture->updateDescriptor();
			}
		}

		void Material::addToBuffer(VkCommandBuffer buffer) {
			vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines[this->pipelineIndex]);
			vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layouts[this->pipe->layoutIndex], 0, 1, &descriptor_set[this->descriptorIndex], 0, nullptr);
		}

		void Material::destroy() {
			destroyDescriptorSet(this->descriptorIndex);
			destroyPipeline(this->pipelineIndex);
			destroyPipelineLayout(this->pipe->layoutIndex);
		}

		bool Material::operator==(const Material& material) {
			return material.color == this->color && material.texture == this->texture;
		}
	}
}