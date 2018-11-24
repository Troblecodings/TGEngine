#include "Actor.hpp"

std::vector<Actor*> actors;

void addActor(Actor* actor) {
	TG_VECTOR_APPEND(actors, actor);
	actor->_impl_array_pos = last_size;
}

void removeActor(Actor* actor) {
	actors.erase(actors.begin() + (actor->_impl_array_pos - 1));
}