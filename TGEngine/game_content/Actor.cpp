#include "Actor.hpp"

std::vector<Actor*> actors;

Actor::Actor(Mesh* mesh) : mesh(mesh) {
	ASSERT_NONE_NULL_DB(mesh, "Mesh is null in actor!", TG_ERR_MESH_NULL_IN_ACTOR)
	Actor* act = this;
	TG_VECTOR_APPEND(actors, &act);
	this->_impl_array_pos = last_size;
}

Actor::~Actor() {
	actors.erase(actors.begin() + (this->_impl_array_pos - 1));
}