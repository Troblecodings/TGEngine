#include "Material.hpp"
#include "Light.hpp"

namespace tge {
	namespace gmc {

		struct _Data {
			glm::vec4 color;
			uint32_t lightCount;
		};

		std::vector<Material*> materiallist;
		std::vector<RenderOffsets> render_offset;

		Material::Material(tex::Texture* texture, glm::vec4 color) {
			this->texture = texture;
			this->setColor(color);

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

			// TODO FIX THIS MESS
			VkSpecializationMapEntry* mapEntrys = new VkSpecializationMapEntry[5];
			for (uint32_t i = 0; i < 4; i++) {
				mapEntrys[i].constantID = i;
				mapEntrys[i].offset = i * sizeof(float);
				mapEntrys[i].size = sizeof(float);
			}

			mapEntrys[4].constantID = 4;
			mapEntrys[4].offset = 4 * sizeof(float);
			mapEntrys[4].size = sizeof(uint32_t);

			_Data data = {
				this->color,
				lightCount
			};

			VkSpecializationInfo pSpecialization;
			pSpecialization.mapEntryCount = 4;
			pSpecialization.pData = &data;
			pSpecialization.dataSize = sizeof(float) * 4 + sizeof(uint32_t);
			pSpecialization.pMapEntries = mapEntrys;
			this->pipe->shader->pSpecializationInfo = &pSpecialization;
			//

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
				this->texture->updateDescriptor();
			}
		}

		void Material::setColor(glm::vec4 color) {
			this->color = color;
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