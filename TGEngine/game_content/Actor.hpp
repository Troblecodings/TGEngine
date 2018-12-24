#pragma once

#include "Mesh.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include "../Error.hpp"
#include "../util/VectorUtil.hpp"

struct Actor
{
	Actor(Mesh* mesh);
	~Actor();

	Actor* parent;
	size_t _impl_array_pos;
	glm::mat4 model_matrix;
	Mesh* mesh;
};

extern std::vector<Actor*> actors;