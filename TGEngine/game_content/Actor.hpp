#pragma once

#include "Mesh.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include "../Error.hpp"
#include "../util/VectorUtil.hpp"

struct Actor
{
	Actor(Mesh* mesh) : mesh(mesh) {
#ifdef DEBUG
		if (!mesh) {
			std::cout << "Mesh is null";
			TGERROR(MESH_NULL_IN_ACTOR)
		}
#endif // DEBUG
		addActor(this);
	}

	~Actor() {
		removeActor(this);
	}

	Actor* parent;
	size_t _impl_array_pos;
	glm::mat4 translate;
	glm::mat4 rotate;
	glm::mat4 scale;
	Mesh* mesh;
};

extern std::vector<Actor*> actors;

void addActor(Actor* actor);

void removeActor(Actor* actor);