#pragma once

#include "Mesh.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include "../Error.hpp"
#include "../util/VectorUtil.hpp"

struct Mesh;

struct Actor
{
	Mesh* mesh;
	Actor* parent;
	size_t _impl_array_pos;
	glm::mat4 model_matrix = glm::mat4(1.0f);

	void applyPretransform();

	Actor* preRotate(float angle, float x, float y, float z);
	Actor* preScale(float x, float y, float z);
	Actor* prePos(float x, float y, float z);
};

extern std::vector<Actor> actors;

Actor* createActor(Mesh* actor);