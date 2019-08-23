#include "Actor.hpp"

namespace tge {
	namespace gmc {

		std::vector<Actor*> actors;

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
		void AABB::print()
		{
			OUT_LV_DEBUG("AABB [ min={ x=" << this->min.x << ", y=" << this->min.y << ", z=" << this->min.z << " }, max { x=" << this->max.x << ", y=" << this->max.y << ", z=" << this->max.z << "}]")
		}
	}
}
