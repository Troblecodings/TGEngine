#pragma once

#include "../pipeline/buffer/Texturebuffer.hpp"
#include "../pipeline/Pipe.hpp"
#include "../resources/ShaderPipes.hpp"

namespace tge {
	namespace gmc {

		enum MaterialType {
			BASIC_MAT_TYPE,
			UI_MAT_TYPE
		};

		/*
		 * Material can hold a texture and a color
		 * -> Can not be changed in vulkan runtime
		 * -> needs pipeline recreation
		 */
		SINCE(0, 0, 4)
			class Material {

			protected:
				ShaderPipe* pipe = nullptr;
				glm::vec4 color = glm::vec4(1.0f);
				tex::Texture* texture = nullptr;

				uint32_t pipelineIndex = 0;
				uint32_t descriptorIndex = 0;

				MaterialType type = BASIC_MAT_TYPE;

			public:

				Material(tex::Texture* texture) : Material(texture, glm::vec4(1.0f)) {}
				Material(glm::vec4 color) : Material(nullptr, color) {}
				Material(tex::Texture* texture, glm::vec4 color);
				Material(ShaderPipe* pipe);

				virtual void createMaterial();

				void addToBuffer(VkCommandBuffer buffer);

				void setColor(glm::vec4 color);

				MaterialType getType() { return this->type; };

				void destroy();

				bool operator==(const Material& material);
		};

		/*
		 * Defines which part of the buffer has which material
		 */
		SINCE(0, 0, 4)
		struct RenderOffsets {

			uint32_t material; // index in the @materials array of the material to use

			uint32_t size; // count of vertices to draw for this matirial
			uint32_t offset; // the offset at wich this material starts (global)
		};

		extern std::vector<Material*> materiallist;
		extern std::vector<RenderOffsets> render_offset;
	}
}
