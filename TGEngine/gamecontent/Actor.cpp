#include "Actor.hpp"

std::vector<Actor> actors;

Actor* createActor(Mesh* msh) {
	TG_VECTOR_APPEND_NORMAL(actors, { msh })
		actors[last_size]._impl_array_pos = last_size;
	return &actors[last_size];
}

void Actor::applyPretransform() {
	for(size_t i = 0; i < mesh->vertices.size(); i++) {
		mesh->vertices[i] = {
			glm::vec3(this->model_matrix * glm::vec4(mesh->vertices[i].position, 1)),
			mesh->vertices[i].uv,
			mesh->vertices[i].normal
		};
	}
}

Actor* Actor::preRotate(float angle, float x, float y, float z) {
	this->model_matrix = glm::rotate(this->model_matrix, angle, glm::vec3(x, y, z));
	return this;
}

Actor* Actor::preScale(float x, float y, float z) {
	this->model_matrix = glm::scale(this->model_matrix, glm::vec3(x, y, z));
	return this;
}

Actor* Actor::prePos(float x, float y, float z) {
	this->model_matrix = glm::translate(this->model_matrix, glm::vec3(x, y, z));
	return this;
}
