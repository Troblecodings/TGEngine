#include "Material.hpp"

namespace tge {
	namespace tex {

		std::vector<Material*> materiallist;
		std::vector<RenderOffsets> render_offset;

		Material::Material(Texture* texture, glm::vec4 color)
		{
			this->texture = texture;
			this->color = color;

			VkSpecializationMapEntry* map_entrys = new VkSpecializationMapEntry[4];
			for (uint32_t i = 0; i < 4; i++)
			{
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

			if (texture == nullptr) {
				this->pipe = &TexturedBasicPipe;
			}
			else {
				this->pipe = &TexturedBasicPipe;
			}
		}

		Material::Material(ShaderPipe* pipe)
		{
			this->pipe = pipe;
		}

		void Material::createMaterial()
		{
			this->pipe->precreation();
			this->pipelineIndex = createPipeline(this->pipe->layoutIndex);

			this->descriptorIndex = createDescriptorSet(this->pipe->layoutIndex);
			camera_uniform.descriptor.descriptorset = light_buffer.descriptor.descriptorset = this->descriptorIndex;

			camera_uniform.descriptor.binding = 0;
			camera_uniform.updateDescriptor();

			light_buffer.descriptor.binding = 1;
			light_buffer.updateDescriptor();

			if (this->texture != nullptr) {
				textureDescriptor.descriptorset = this->descriptorIndex;
				this->texture->vulkanTexture->updateDescriptor();
			}
		}

		void Material::addToBuffer(VkCommandBuffer buffer)
		{
			vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines[this->pipelineIndex]);
			vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layouts[this->pipe->layoutIndex], 0, 1, &descriptor_set[this->descriptorIndex], 0, nullptr);
		}

		/*void Material::createUIMaterial()
		{
			vlib_depth_stencil_state.depthTestEnable = VK_FALSE;
			vlib_rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
			Material::createMaterial();
			vlib_rasterization_state.cullMode = VK_CULL_MODE_FRONT_BIT;
			vlib_depth_stencil_state.depthTestEnable = VK_TRUE;
			ui_camera_uniform.descriptor.descriptorset = this->descriptor_index;
			ui_camera_uniform.descriptor.binding = 0;
			ui_camera_uniform.updateDescriptor();
		}*/

		void Material::destroy()
		{
			destroyDescriptorSet(this->descriptorIndex);
			destroyPipeline(this->pipelineIndex);
			destroyPipelineLayout(this->pipe->layoutIndex);
		}

		bool Material::operator==(const Material& material)
		{
			return material.color == this->color && material.texture == this->texture;
		}
	}
}