#pragma once

#include "Material.hpp"

namespace tge {
	namespace gmc {

		class UIMaterial : public Material {

		public:
			UIMaterial(tex::TextureOutput* texture) : UIMaterial(texture, glm::vec4(1.0f)) {}
			UIMaterial(glm::vec4 color) : UIMaterial(nullptr, color) {}
			UIMaterial(tex::TextureOutput* texture, glm::vec4 color);

			virtual void createMaterial() override;
		};
	}
}