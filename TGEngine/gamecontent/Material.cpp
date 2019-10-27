#include "Material.hpp"
#include "../pipeline/Device.hpp"

namespace tge::gmc{
	
	VkBuffer materialUBO;
	VkDeviceMemory materialMemory;

	void initMaterials() {

	}

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
		uint32_t comp_size = sizeof(Material) * size;
		vkMapMemory(device, materialMemory, 0, comp_size, 0, &ptr);
		memcpy(ptr, material, comp_size);
		vkUnmapMemory(device, materialMemory);
	}

}
