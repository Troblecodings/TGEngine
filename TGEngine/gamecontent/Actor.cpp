#include "Actor.hpp"

namespace tge {
	namespace gmc {

        std::vector<Model*> models;

		void AABB::print()
		{
			OUT_LV_DEBUG("AABB [ min={ x=" << this->min.x << ", y=" << this->min.y << ", z=" << this->min.z << " }, max { x=" << this->max.x << ", y=" << this->max.y << ", z=" << this->max.z << "}]")
		}

		glm::mat4 Actor::transforms()
		{
			Actor* pr = parent;
			glm::mat4 mat = localTransform();
			while (pr)
			{
				mat = pr->matrix * mat;
				pr = pr->parent;
			}
			return mat;
		}

		glm::mat4 Actor::localTransform()
		{
			return glm::translate(glm::mat4(1), this->translation) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1), this->scale) * matrix;
		}
	}
}
