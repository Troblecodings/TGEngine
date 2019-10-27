#include "ModelLoader.hpp"
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
//#define TINYGLTF_ENABLE_DRACO
#include "../tinygltf/tiny_gltf.h"
#include <glm/gtc/type_ptr.hpp>
#include "../resources/ShaderPipes.hpp"
#include "../util/Math.hpp"
#include "../vlib/VulkanImage.hpp"

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
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
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
			return VK_FILTER_LINEAR;
		}
		// [END]

			// Load sampler
		static void loadSampler(tinygltf::Model* gltfModel, gmc::Model* model) {
			model->samplers.resize(gltfModel->samplers.size());
			for (size_t i = 0; i < model->samplers.size(); i++)
			{
				Sampler sampler = gltfModel->samplers[i];
				OUT_LV_DEBUG("Load sampler " << sampler.name << " id = " << i)
				vlibSamplerCreateInfo.minFilter = getVkFilterMode(sampler.minFilter);
				vlibSamplerCreateInfo.magFilter = getVkFilterMode(sampler.magFilter);
				vlibSamplerCreateInfo.addressModeU = getVkWrapMode(sampler.wrapS);
				vlibSamplerCreateInfo.addressModeV = getVkWrapMode(sampler.wrapT);
				vlibSamplerCreateInfo.addressModeW = vlibSamplerCreateInfo.addressModeV;
				//tex::createSampler(&model->samplers[i]); TODO Sampler creation
			}
		}

		// Loads textures
		static void loadTextures(tinygltf::Model* gltfModel, gmc::Model* model) {
			for each (tinygltf::Texture tex in gltfModel->textures) {
				OUT_LV_DEBUG("Load texture " << tex.name)
					tinygltf::Image image = gltfModel->images[tex.source];
				// Load image data
				OUT_LV_DEBUG(image.image.size());
				//TG_VECTOR_APPEND_NORMAL(model->textures, tex::Texture(image.width, image.height));
				//model->textures[lastSize].imageData = new uint8_t[image.width * image.height * 4];
				if (image.component == 4) {
					//memcpy(model->textures[lastSize].imageData, image.image.data(), image.width * image.height * 4);
				}
				else {
					for (size_t i = 0; i < image.width * image.height; i++)
					{
					//	model->textures[lastSize].imageData[i * 4] = image.image[i * 4];
					//	model->textures[lastSize].imageData[i * 4 + 1] = image.image[i * 4 + 1];
					//	model->textures[lastSize].imageData[i * 4 + 2] = image.image[i * 4 + 2];
					//	model->textures[lastSize].imageData[i * 4 + 3] = 255;
					}
				}
				if (tex.sampler > -1) {
					// Set custom sampler
					OUT_LV_DEBUG("Set custom sampler " << tex.sampler)
						//model->textures[lastSize].sampler = model->samplers[tex.sampler];
				}
				//model->textures[lastSize].initTexture();
			}
		}

		// Loading materials
		static void loadMaterials(tinygltf::Model* gltfModel, gmc::Model* model) {
			if (gltfModel->materials.size() < 1) {
				// TODO Make this default Material ...
				// TODO Creation for materials
				return;
			}
			for each (tinygltf::Material mat in gltfModel->materials) {
				OUT_LV_DEBUG("Load material " << mat.name)
				auto colorTex = mat.values.find("baseColorTexture");
				auto colorFactor = mat.values.find("baseColorFactor");

				auto doubleSided = mat.additionalValues.find("doubleSided");

				if (colorTex != mat.values.end()) {
					// TODO 
				}
				else if (colorFactor != mat.values.end()) {
					// TODO 
				}
				else {
					// TODO Make this default Material ...
					// TODO 
				}

				if (doubleSided != mat.additionalValues.end());
				// TODO 

#ifdef DEBUG
				for (auto ptr = mat.values.begin(); ptr != mat.values.end(); ptr++) {
					OUT_LV_DEBUG(ptr->first << " = " << ptr->second.string_value)
				}
				OUT_LV_DEBUG("Additional")
					for (auto ptr = mat.additionalValues.begin(); ptr != mat.additionalValues.end(); ptr++) {
						OUT_LV_DEBUG(ptr->first << " = " << ptr->second.string_value)
					}
#endif // DEBUG
			}
		}

#define FIND(name, type, cname) const type* name##Buffer = nullptr;\
if (prim.attributes.find(cname) != prim.attributes.end()) {\
	const tinygltf::Accessor& name##Accessor = gltfModel->accessors[prim.attributes.find(cname)->second];\
	const tinygltf::BufferView& name##View = gltfModel->bufferViews[name##Accessor.bufferView];\
	name##Buffer = reinterpret_cast<const type*>(&(gltfModel->buffers[name##View.buffer].data[name##Accessor.byteOffset + name##View.byteOffset]));\
}

#define ADD_INDEX(type) {const type* buf = static_cast<const type*>(indexBuffer);\
for (size_t i = 0; i < indexAccessor.count; i++)\
{\
	mesh->indices.push_back((uint32_t) buf[i]);\
}}

		static void loadNodes(tinygltf::Model* gltfModel, gmc::Model* model) {
			model->actors.resize(gltfModel->nodes.size());

			for (size_t i = 0; i < model->actors.size(); i++)
			{
				tinygltf::Node node = gltfModel->nodes[i];

				tge::gmc::Actor* actor = new tge::gmc::Actor();
				tge::gmc::Mesh* mesh = actor->mesh = new tge::gmc::Mesh();
				model->actors[i] = actor;

				if (node.matrix.size() == 16) {
					actor->matrix = glm::make_mat4(node.matrix.data());
				}
				if (node.translation.size() == 3) {
					actor->translation = glm::make_vec3((float*)node.translation.data());
				}
				if (node.rotation.size() == 4) {
					actor->rotation = glm::make_quat((float*)node.rotation.data());
				}
				if (node.scale.size() == 3) {
					actor->scale = glm::make_vec3((float*)node.scale.data());
				}

				if (node.mesh > -1) {
					const tinygltf::Mesh msh = gltfModel->meshes[gltfModel->nodes[i].mesh];
					for each (tinygltf::Primitive prim in msh.primitives)
					{
#ifdef DEBUG
						for (auto ptr = prim.attributes.begin(); ptr != prim.attributes.end(); ptr++) {
							OUT_LV_DEBUG(ptr->first << " = " << ptr->second)
						}
#endif // DEBUG
						if (prim.attributes.find("POSITION") == prim.attributes.end()) {
							OUT_LV_DEBUG("Pos attributes not found!");
							continue;
						}

						const tinygltf::Accessor& posAccessor = gltfModel->accessors[prim.attributes.find("POSITION")->second];
						const tinygltf::BufferView& posView = gltfModel->bufferViews[posAccessor.bufferView];
						const float* posBuffer = reinterpret_cast<const float*>(&(gltfModel->buffers[posView.buffer].data[posAccessor.byteOffset + posView.byteOffset]));

						FIND(normal, float, "NORMAL");
						FIND(uv, float, "TEXCOORD_0");

						// TODO TANGENT? UV_1? JOINTS? WEIGHT? ...

						if (prim.indices > -1) {
							for (size_t i = 0; i < posAccessor.count; i++)
							{
								TGVertex vert = {
									glm::vec4(glm::make_vec3(&posBuffer[i * 3]), 1),
									uvBuffer ? glm::make_vec2(&uvBuffer[i * 2]) : glm::vec2(0),
									normalBuffer ? glm::normalize(glm::make_vec3(&normalBuffer[i * 3])) : glm::vec3(0)
								};
								mesh->vertices.push_back(vert);
							}

							const tinygltf::Accessor& indexAccessor = gltfModel->accessors[prim.indices];
							const tinygltf::BufferView& indexView = gltfModel->bufferViews[indexAccessor.bufferView];
							const void* indexBuffer = &(gltfModel->buffers[indexView.buffer].data[indexAccessor.byteOffset + indexView.byteOffset]);

							switch (indexAccessor.componentType)
							{
							case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
								ADD_INDEX(uint32_t)
									break;
							case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
								ADD_INDEX(uint16_t)
									break;
							case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
								ADD_INDEX(uint8_t)
									break;
							default:
								OUT_LV_DEBUG("Index type not supported!")
									TGERROR(TG_ERR_INDEX_TYPE_NOT_SUPPORTED)
									break;
							}
						}
						else {
							for (size_t i = 0; i < posAccessor.count; i++)
							{
								TGVertex vert = {
									glm::vec4(glm::make_vec3(&posBuffer[i * 3]), 1),
									uvBuffer ? glm::make_vec2(&uvBuffer[i * 2]) : glm::vec2(0),
									normalBuffer ? glm::normalize(glm::make_vec3(&normalBuffer[i * 3])) : glm::vec3(0)
								};
								mesh->add(vert);
							}
						}

					}
				}
			}
			for (size_t i = 0; i < gltfModel->nodes.size(); i++)
			{
				for (size_t j = 0; j < gltfModel->nodes[i].children.size(); j++)
				{
					model->actors[gltfModel->nodes[i].children[j]]->parent = model->actors[i];
				}
			}

		}

		void loadGltf(char* name, gmc::Model* model) {
			TinyGLTF loader;
			tinygltf::Model gltfModel;

			bool binary = strstr(name, ".glb");

			std::string err;
			std::string warn;

			// TODO use pak loading when avalaible
			bool check = binary ? loader.LoadBinaryFromFile(&gltfModel, &err, &warn, name) : loader.LoadASCIIFromFile(&gltfModel, &err, &warn, name);

			if (!err.empty()) {
				std::cout << err << std::endl;
			}

			if (!warn.empty()) {
				OUT_LV_DEBUG("Warn: " << warn)
			}

			if (!check) {
				OUT_LV_DEBUG("Error loading " << name << " gltf file")
				TGERROR(TG_ERR_DB_FILE_NOT_FOUND)
			}

			loadSampler(&gltfModel, model);
			loadTextures(&gltfModel, model);
			loadMaterials(&gltfModel, model);
			loadNodes(&gltfModel, model);

			if (model->actors.size() < 1 || !model->actors[0]->mesh || model->actors[0]->mesh->vertices.size() < 1)
				return;
			// AABB Calculation
			tge::gmc::AABB aabb = { model->actors[0]->mesh->vertices[0].position,  model->actors[0]->mesh->vertices[0].position };

			for (size_t j = 0; j < model->actors.size(); j++)
			{
				for (size_t i = 0; i < model->actors[j]->mesh->vertices.size(); i++)
				{
					glm::vec3 vert = model->actors[j]->mesh->vertices[i].position;
					aabb.max.y = TGE_MAX(aabb.max.y, vert.y);
					aabb.max.z = TGE_MAX(aabb.max.z, vert.z);
					aabb.max.x = TGE_MAX(aabb.max.x, vert.x);

					aabb.min.x = TGE_MIN(aabb.min.x, vert.x);
					aabb.min.y = TGE_MIN(aabb.min.y, vert.y);
					aabb.min.z = TGE_MIN(aabb.min.z, vert.z);
				}
			}
			model->aabb = aabb;
#ifdef DEBUG
			aabb.print();
#endif // DEBUG

			glm::vec3 offset = glm::vec3(0) - (((aabb.max - aabb.min) / 2.0f) + aabb.min);
		}
	}
}