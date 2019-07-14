#include "ModelLoader.hpp"
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#include "../tinygltf/tiny_gltf.h"

namespace tge {
	namespace mdl {

		using namespace tinygltf;

		static void loadTextures() {

		}

		void loadGltf(char* name, gmc::Mesh* mesh) {
			TinyGLTF loader;
			Model model;

			bool binary = strstr(name, "glb");

			std::string err;
			std::string warn;

			bool check = binary ? loader.LoadBinaryFromFile(&model, &err, &warn, name) : loader.LoadASCIIFromFile(&model, &err, &warn, name);

			if(!err.empty()) {
				std::cout << err << std::endl;
			}

			if(!warn.empty()) {
				OUT_LV_DEBUG("Warn " << warn)
			}

			if(!check) {
				OUT_LV_DEBUG("Error loading " << name << " gltf file")
				TGERROR(TG_ERR_DB_FILE_NOT_FOUND)
			}
		}
	}
}