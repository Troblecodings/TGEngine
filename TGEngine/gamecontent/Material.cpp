#include "Material.hpp"
#include "../pipeline/Device.hpp"
#include "../pipeline/buffer/UniformBuffer.hpp"

namespace tge::gmc{
	
	using namespace buf;

	void loadMaterials(File file, ResourceDescriptor* desc, uint32_t size, Material* material) {
		for (uint32_t i = 0; i < size; i++)
		{
			if (ftell(file) != desc[i].offset)
				fseek(file, desc[i].offset, SEEK_SET);
			fread(&material[i], sizeof(char), desc[i].size, file);
		}
	}

	void createMaterials(Material* material, uint32_t size) {
		void* ptr;
		BufferObject materials = buffers[MATERIAL_BUFFER];
		uint32_t comp_size = sizeof(Material) /* size of object*/ * size;
		vkMapMemory(device, materials.memory, 12, comp_size, 0, &ptr);
		memcpy(ptr, material, comp_size);
		vkUnmapMemory(device, materials.memory);
	}

}
