#pragma once

#include "Material.hpp"

namespace tge {
	namespace tex {

		class UIMaterial : public Material {

		public:
			UIMaterial(Texture* texture) : UIMaterial(texture, glm::vec4(1.0f)) {}
			UIMaterial(glm::vec4 color) : UIMaterial(nullptr, color) {}
			UIMaterial(Texture* texture, glm::vec4 color);

			virtual void createMaterial() override;
		};
	}
}