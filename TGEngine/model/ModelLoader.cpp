#include "ModelLoader.hpp"
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#include "../tinygltf/tiny_gltf.h"

namespace tge {
	namespace mdl {

		using namespace tinygltf;

	// MIT License by Sascha Willems
	// Changes: made the methodes static
		static VkSamplerAddressMode getVkWrapMode(int32_t wrapMode)
		{
			switch (wrapMode) {
			case 10497:
				return VK_SAMPLER_ADDRESS_MODE_REPEAT;
			case 33071:
				return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			case 33648:
				return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
			}
		}

		static VkFilter getVkFilterMode(int32_t filterMode)
		{
			switch (filterMode) {
			case 9728:
				return VK_FILTER_NEAREST;
			case 9729:
				return VK_FILTER_LINEAR;
			case 9984:
				return VK_FILTER_NEAREST;
			case 9985:
				return VK_FILTER_NEAREST;
			case 9986:
				return VK_FILTER_LINEAR;
			case 9987:
				return VK_FILTER_LINEAR;
			}
		}
    // [END]

		// Load sampler
		static void loadSampler(tinygltf::Model* model, gmc::Mesh* mesh) {
			mesh->samplers.resize(model->samplers.size());
			for (size_t i = 0; i < mesh->samplers.size(); i++)
			{
				Sampler sampler = model->samplers[i];
				OUT_LV_DEBUG("Load sampler " << sampler.name << " id = " << i)
				vlibSamplerCreateInfo.minFilter = getVkFilterMode(sampler.minFilter);
				vlibSamplerCreateInfo.magFilter = getVkFilterMode(sampler.magFilter);
				vlibSamplerCreateInfo.addressModeU = getVkWrapMode(sampler.wrapS);
				vlibSamplerCreateInfo.addressModeV = getVkWrapMode(sampler.wrapT);
				vlibSamplerCreateInfo.addressModeW = vlibSamplerCreateInfo.addressModeV;
				tex::createSampler(&mesh->samplers[i]);
			}
		}

		// Loads textures
		static void loadTextures(tinygltf::Model* model, gmc::Mesh* mesh) {
			for each(tinygltf::Texture tex in model->textures) {
				OUT_LV_DEBUG("Load texture " << tex.name)
				tinygltf::Image image = model->images[tex.source];
				// Load image data
				tex::Texture texture = tex::Texture(image.image.data(), image.width, image.height);
				OUT_LV_DEBUG(texture.getWidth() << " " << texture.getHeight())
				if (tex.sampler > -1) {
					// Set custom sampler
					OUT_LV_DEBUG("Set custom sampler " << tex.sampler)
					texture.sampler = &mesh->samplers[tex.sampler];
				}
				mesh->textures.push_back(texture);
			}
		}

		// Loading materials
		static void loadMaterials(tinygltf::Model* model, gmc::Mesh* mesh) {
			for each (tinygltf::Material mat in model->materials) {
				OUT_LV_DEBUG("Load material " << mat.name)
				gmc::Material material = gmc::Material(&mesh->textures[mat.values.find("baseColorTexture")->second.TextureIndex()]);
				mesh->materials.push_back(material);
				for (auto ptr = mat.values.begin(); ptr != mat.values.end(); ptr++) {
					OUT_LV_DEBUG(ptr->first << " = " << ptr->second.string_value)
				}
				OUT_LV_DEBUG("Additional")
				for (auto ptr = mat.additionalValues.begin(); ptr != mat.additionalValues.end(); ptr++) {
					OUT_LV_DEBUG(ptr->first << " = " << ptr->second.string_value)
				}
			}
		}

		void loadGltf(char* name, gmc::Mesh* mesh) {
			TinyGLTF loader;
			tinygltf::Model model;

			bool binary = strstr(name, "glb");

			std::string err;
			std::string warn;

			bool check = binary ? loader.LoadBinaryFromFile(&model, &err, &warn, name) : loader.LoadASCIIFromFile(&model, &err, &warn, name);

			if(!err.empty()) {
				std::cout << err << std::endl;
			}

			if(!warn.empty()) {
				OUT_LV_DEBUG("Warn: " << warn)
			}

			if(!check) {
				OUT_LV_DEBUG("Error loading " << name << " gltf file")
				TGERROR(TG_ERR_DB_FILE_NOT_FOUND)
			}

			loadSampler(&model, mesh);
			loadTextures(&model, mesh);
			loadMaterials(&model, mesh);
		}
	}
}