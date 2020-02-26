#include "Material.hpp"
#include "../pipeline/Device.hpp"
#include "../pipeline/buffer/UniformBuffer.hpp"

namespace tge::gmc {

	using namespace buf;

	Material createdMaterials[256];

	void loadMaterials(File file, ResourceDescriptor* desc, uint32_t size) {
		for (uint32_t i = 0; i < size; i++) {
			if (ftell(file) != desc[i].offset)
				fseek(file, (long)desc[i].offset, SEEK_SET);
			fread(&createdMaterials[i], sizeof(char), desc[i].size, file);
		}
	}

}
