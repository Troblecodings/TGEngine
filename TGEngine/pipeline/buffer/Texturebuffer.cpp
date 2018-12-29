 child 202990, depot 202973
[2018-12-29 17:33:27] Dependency added: parent 202970, child 202990, depot 202975
[2018-12-29 17:33:27] Dependency added: parent 202970, child 202990, depot 212931
[2018-12-29 17:33:27] Dependency added: parent 202970, child 202990, depot 202998
[2018-12-29 17:33:27] Dependency added: parent 202970, child 212910, depot 202972
[2018-12-29 17:33:27] Dependency added: parent 202970, child 212910, depot 202978
[2018-12-29 17:33:27] Dependency added: parent 202970, child 212910, depot 202998
[2018-12-29 17:33:27] Dependency added: parent 228980, child 252950, depot 228983
[2018-12-29 17:33:27] Dependency added: parent 228980, child 252950, depot 228986
[2018-12-29 17:33:27] Dependency added: parent 228980, child 252950, depot 228990
[2018-12-29 17:33:27] Dependency added: parent 228980, child 252950, depot 229033
[2018-12-29 17:33:27] Dependency added: parent 228980, child 368340, depot 228987
[2018-12-29 17:33:27] Dependency added: parent 228980, child 368340, depot 228990
[2018-12-29 17:33:27] Dependency added: parent 228980, child 428660, depot 228986
[2018-12-29 17:33:27] Dependency added: parent 228980, child 428660, depot 228990
[2018-12-29 17:33:27] Dependency added: parent 228980, child 750920, depot 228986
[2018-12-29 17:33:27] Dependency added: parent 228980, child 750920, depot 228990
[2018-12-29 17:33:27] Dependency added: parent 228980, child 845070, depot 228986
[2018-12-29 17:33:27] Dependency added: parent 228980, child 845070, depot 228990
[2018-12-29 17:33:27] Dependency added: parent 228980, child 845070, depot 229005


[2018-12-29 17:33:40] Loaded 55 apps from install folder "D:\Steam\steamapps\appmanifest_*.acf".
[2018-12-29 17:33:40] Loaded 0 apps from install folder "D:\SteamLibrary\steamapps\appmanifest_*.acf".


[2018-12-29 17:58:06] Loaded 55 apps from install folder "D:\Steam\steamapps\appmanifest_*.acf".
[2018-12-29 17:58:06] Loaded 0 apps from install folder "D:\SteamLibrary\steamapps\appmanifest_*.acf".
[2018-12-29 17:58:25] Dependency added: parent 70, child 10, depot 1
[2018-12-29 17:58:25] Dependency added: parent 70, child 10, depot 3
[2018-12-29 17:58:25] Dependency added: parent 70, child 10, depot 2
[2018-12-29 17:58:25] Dependency added: parent 10, child 80, depot 11
[2018-12-29 17:58:25] Dependency added: parent 70, child 80, depot 1
[2018-12-29 17:58:25] Dependency added: parent 70, child 80, depot 3
[2018-12-29 17:58:25] Dependency added: parent 70, child 80, depot 2
[2018-12-29 17:58:25] Dependency added: parent 10, child 100, depot 11
[2018-12-29 17:58:25] Dependency added: parent 70, child 100, depot 1
[2018-12-29 17:58:25] Dependency added: parent 70, child 100, depot 3
[2018-12-29 17:58:25] Dependency added: parent 70, child 100, depot 2
[2018-12-29 17:58:25] Dependency added: parent 42700, child 42710, depot 42702
[2018-12-29 17:58:25] Dependency added: parent 42700, child 42710, depot 42706
[2018-12-29 17:58:25] Dependency added: parent 202970, child 202990, depot 202972
[2018-12-29 17:58:25] Dependency added: parent 202970, child 202990, depot 202973
[2018-12-29 17:58:25] Dependency added: parent 202970, child 202990, depot 202975
[2018-12-29 17:58:25] Dependency added: parent 202970, child 202990, depot 212931
[2018-12-29 17:58:25] Dependency added: parent 202970, child 202990, depot 202998
[2018-12-29 17:58:25] Dependency added: parent 202970, child 212910, depot 202972
[2018-12-29 17:58:25] Dependency added: parent 202970, child 212910, depot 202978
[2018-12-29 17:58:25] Dependency added: parent 202970, child 212910, depot 202998
[2018-12-29 17:58:25] Dependency added: parent 228980, child 252950, depot 228983
[2018-12-29 17:58:25] Dependency added: parent 228980, child 252950, depot 228986
[2018-12-29 17:58:25] Dependency added: parent 228980, child 252950, depot 228990
[2018-12-29 17:58:25] Dependency added: parent 228980, child 252950, depot 229033
[2018-12-29 17:58:25] Dependency added: parent 228980, child 368340, depot 228987
[2018-12-29 17:58:25] Dependency added: parent 228980, child 36,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		};
	}
	VkWriteDescriptorSet descriptor_writes = {
		VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		nullptr,
		descriptor_set,
		texture_descriptor.binding,
		0,
		MAX_TEXTURES,
		VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		sampler_array.data(),
		nullptr,
		nullptr
	};
	vkUpdateDescriptorSets(device, 1, &descriptor_writes, 0, nullptr);
}

void destroyBufferofTexture(Texture* tex) {
	vkFreeMemory(device, tex->buffer_memory, nullptr);
	vkDestroyBuffer(device, tex->buffer, nullptr);
}


void destroyTexture(Texture* tex) {
	vkDestroyImageView(device, tex->image_view, nullptr);
	vkFreeMemory(device, tex->d_memory, nullptr);
	vkDestroyImage(device, tex->image, nullptr);
}

void destroyAllTextures() {
	last_result = vkDeviceWaitIdle(device);
	HANDEL(last_result)

	vkDestroySampler(device, tex_image_sampler, nullptr);
	for each(Texture* tex in texture_buffers) {
		destroyTexture(tex);
	}
}