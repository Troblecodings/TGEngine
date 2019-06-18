#include "Material.hpp"

namespace tge {
	namespace tex {

		std::vector<Material*> materials;
		std::vector<RenderOffsets> render_offset;

		Material::Material(Texture* texture, glm::vec4 color)
		{
			this->texture = texture;
			this->color = color;

			VkSpecializationMapEntry* map_entrys = new VkSpecializationMapEntry[4];
			for (size_t i = 0; i < 4; i++)
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
				Material::Material(TexturedBasicInput, TexturedBasicInputCount, TexturedBasicShader, TexturedBasicShaderCount);
			}
			else {
				Material::Material(TexturedBasicInput, TexturedBasicInputCount, TexturedBasicShader, TexturedBasicShaderCount);
			}
		}

		Material::Material(const VkVertexInputAttributeDescription* inputAttributes, const uint32_t inputCount, VkPipelineShaderStageCreateInfo* shader, const uint32_t shaderCount)
		{
			this->inputAttributes = inputAttributes;
			this->inputCount = inputCount;
			this->shader = shader;
			this->shaderCount = shaderCount;

			materials.push_back(this);
		}

		void Material::createMaterial()
		{
			createPipelineLayout(1, &descriptorSetLayouts[createDesctiptorLayout()]);
			this->layoutIndex = last_size;

			this->descriptor_index = textureDescriptor.descriptorset = light_buffer.descriptor.descriptorset = camera_uniform.descriptor.descriptorset = createDescriptorSet(this->layout_index);

			vlib_vertex_input_state.pVertexAttributeDescriptions = this->inputAttributes;
			vlib_vertex_input_state.vertexAttributeDescriptionCount = this->inputCount;
			vlib_graphics_pipeline_create_info.pStages = this->shader;
			vlib_graphics_pipeline_create_info.stageCount = this->shaderCount;

			createPipeline(this->layout_index);
			this->pipeline_index = last_size;

			camera_uniform.descriptor.binding = 0;
			camera_uniform.updateDescriptor();

			light_buffer.descriptor.binding = 1;
			light_buffer.updateDescriptor();

			this->texture->vulkanTexture->updateDescriptor();
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
			destroyDescriptorSet(this->descriptor_index);
			destroyPipeline(this->pipeline_index);
			destroyPipelineLayout(this->layout_index);
		}

		bool Material::operator==(const Material& material)
		{
			return material.color == this->color && material.texture == this->texture;
		}
	}
}