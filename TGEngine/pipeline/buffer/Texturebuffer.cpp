#include "Texturebuffer.hpp"

namespace tge::tex {

	using namespace tge::nio;

	void createTextures(TextureIn* input, uint32_t size, TextureOutput* output)
	{
		// TODO Default Format checks

		File resc = open("resources\Resources.tgr", "rb");

		VkDescriptorImageInfo* imagedesc = new VkDescriptorImageInfo[size];

		for (uint32_t i = 0; i < size; i++)
		{
			TextureIn tex = input[i];
			TextureOutput out = output[i];

			stbi_uc* buffer = new stbi_uc[tex.size];
			fread(buffer, sizeof(char), tex.size, resc);
			stbi_uc* loaded = stbi_load_from_memory(buffer, tex.size, &out.x, &out.y, &out.comp, STBI_rgb_alpha);
			delete[] buffer;
			
			// Todo do Vulkan stuff
		}
		fclose(resc);

		VkWriteDescriptorSet descwrite;
		descwrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descwrite.dstBinding = 0;
		descwrite.pImageInfo = imagedesc;
		descwrite.descriptorCount = size;
		descwrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		// TODO set descriptor set
		vkUpdateDescriptorSets(device, 1, &descwrite, 0, nullptr);
		delete[] imagedesc;
	}

}