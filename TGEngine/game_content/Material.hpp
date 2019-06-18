#pragma once

#include "../pipeline/buffer/Texturebuffer.hpp"
#include "../pipeline/Pipe.hpp"
#include "../resources/ShaderPipes.hpp"

namespace tge {
	namespace tex {

		/*
		 * Material can hold a texture and a color
		 * -> Can not be changed in vulkan runtime
		 * -> needs pipeline recreation
		 */
		SINCE(0, 0, 4)
		class Material {

		private:
			Texture* texture = nullptr;
			glm::vec4 color;

			ShaderPipe* pipe;

			uint32_t pipelineIndex = 0;

		public:
			Material(Texture* texture) : Material(texture, glm::vec4(1.0f)) {}
			Material(glm::vec4 color) : Material(nullptr, color) {}
			Material(Texture* texture, glm::vec4 color);
			Material(ShaderPipe* pipe);

			virtual void createMaterial();

			void addToBuffer(VkCommandBuffer buffer);

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

		extern std::vector<Material*> materials;
		extern std::vector<RenderOffsets> render_offset;
	}
}
