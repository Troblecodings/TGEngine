/*#include "GLTFLoader.hpp"

namespace tg_model {

	void loadGLTFModel(char* name)
	{
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;

		char * ptr = reinterpret_cast<char*>(nio::readAll(name));
		ASSERT_NONE_NULL(loader.LoadASCIIFromString(&model, nullptr, nullptr, ptr, strlen(ptr), std::string()), "Faild loading [" << name << "]", TG_ERR_MODEL_ERROR)
		for each (tinygltf::Mesh mesh in model.meshes) {

		}
	}

}*/