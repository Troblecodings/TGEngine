#pragma once

#include "Mesh.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include "../Error.hpp"
#include "../util/VectorUtil.hpp"

namespace tge {
	namespace gmc {

		struct AABB {
			glm::vec3 min;
			glm::vec3 max;

			void print();
		};

		struct Mesh;

		class Actor {
		public:
			Mesh* mesh;
			Actor* parent;
			AABB aabb;
			size_t _impl_array_pos;
			glm::mat4 model_matrix = glm::mat4(1.0f);

			std::vector<VkSampler> samplers;
			std::vector<tex::Texture> textures;
			std::vector<Material> materials;
			std::vector<RenderOffsets> offsets;

			void applyPretransform();

			Actor* preRotate(float angle, float x, float y, float z);
			Actor* preScale(float x, float y, float z);
			Actor* prePos(float x, float y, float z);
		};

		extern std::vector<Actor*> actors;
	}
}