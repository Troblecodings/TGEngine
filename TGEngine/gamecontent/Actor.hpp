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
			
			Actor* parent;
			AABB aabb;
			glm::mat4 matrix;

			std::vector<glm::mat4> mats;
			std::vector<Mesh*> meshes;
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