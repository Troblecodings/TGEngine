#pragma once

#include "Mesh.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include "../Error.hpp"
#include "../util/VectorUtil.hpp"
#include "Material.hpp"

namespace tge {
	namespace gmc {

		struct AABB {
			glm::vec3 min;
			glm::vec3 max;

			void print();
		};

		struct Actor {

			Actor* parent;
			Mesh* mesh;
			AABB aabb;

			glm::mat4 matrix = glm::mat4(1);
			glm::quat rotation = glm::quat(0, 0, 0, 0);
			glm::vec3 translation = glm::vec3(0);
			glm::vec3 scale = glm::vec3(1);

			/*
             * Returns the overall transform matrix from this Actor
             */
			SINCE(0, 0, 4)
			glm::mat4 transforms();


			/*
			 * Returns the overall transform matrix from this Actor
			 */
			SINCE(0, 0, 4)
			glm::mat4 localTransform();
		};

		struct Model {
			AABB aabb;

			std::vector<Actor*> actors;
			std::vector<VkSampler> samplers;
			std::vector<tex::TextureOutput> textures;
			std::vector<Material> materials;
			std::vector<RenderOffsets> offsets;
		};

		extern std::vector<Model*> models;
	}
}