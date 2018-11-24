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
	glm::mat4 translate;
	glm::mat4 rotate;
	glm::mat4 scale;
	Mesh* mesh;
};

extern std::vector<Actor*> actors;