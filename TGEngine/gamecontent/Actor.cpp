#include "Actor.hpp"

namespace tge {
	namespace gmc {

		std::vector<Actor*> actors;

		void Actor::applyPretransform() {
			for(size_t i = 0; i < this->meshes.size(); i++) {
				for (size_t j = 0; j < this->meshes[i]->vertices.size(); j++)
				{
					this->meshes[i]->vertices[j].position = glm::vec3(this->mats[i] * this->matrix * glm::vec4(this->meshes[i]->vertices[j].position, 1));
				}
			}
		}

		Actor* Actor::preRotate(float angle, float x, float y, float z) {
			this->matrix = glm::rotate(this->matrix, angle, glm::vec3(x, y, z));
			return this;
		}

		Actor* Actor::preScale(float x, float y, float z) {
			this->matrix = glm::scale(this->matrix, glm::vec3(x, y, z));
			return this;
		}

		Actor* Actor::prePos(float x, float y, float z) {
			this->matrix = glm::translate(this->matrix, glm::vec3(x, y, z));
			return this;
		}

		void AABB::print()
		{
			OUT_LV_DEBUG("AABB [ min={ x=" << this->min.x << ", y=" << this->min.y << ", z=" << this->min.z << " }, max { x=" << this->max.x << ", y=" << this->max.y << ", z=" << this->max.z << "}]")
		}
	}
}
