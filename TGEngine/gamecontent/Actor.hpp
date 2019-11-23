#pragma once

#include "Mesh.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include "../Error.hpp"
#include "../util/VectorUtil.hpp"
#include "Material.hpp"

namespace tge::gmc {

	struct Actor {
		Actor* parent;
		Mesh* mesh;

		std::vector<TGVertex> vertices;
		std::vector<uint32_t> indices;

		glm::mat4 matrix = glm::mat4(1);
		glm::quat rotation = glm::quat(0, 0, 0, 0);
		glm::vec3 translation = glm::vec3(0);
		glm::vec3 scale = glm::vec3(1);
	};

	struct Model {
		std::vector<Actor*> actors;
		std::vector<Material> materials;
	};

	extern std::vector<Model*> models;
}