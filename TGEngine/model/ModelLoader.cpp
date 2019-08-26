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
		static void loadSampler(tinygltf::Model* model, gmc::Actor* mesh) {
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
		static void loadTextures(tinygltf::Model* model, gmc::Actor* mesh) {
			for each (tinygltf::Texture tex in model->textures) {
				OUT_LV_DEBUG("Load texture " << tex.name)
					tinygltf::Image image = model->images[tex.source];
				// Load image data
				OUT_LV_DEBUG(image.image.size());
				TG_VECTOR_APPEND_NORMAL(mesh->textures, tex::Texture(image.width, image.height));
				mesh->textures[lastSize].imageData = new uint8_t[image.width * image.height * 4];
				if (image.component == 4) {
					memcpy(mesh->textures[lastSize].imageData, image.image.data(), image.width * image.height * 4);
				}
				else {
					for (size_t i = 0; i < image.width * image.height; i++)
					{
						mesh->textures[lastSize].imageData[i * 4] = image.image[i * 4];
						mesh->textures[lastSize].imageData[i * 4 + 1] = image.image[i * 4 + 1];
						mesh->textures[lastSize].imageData[i * 4 + 2] = image.image[i * 4 + 2];
						mesh->textures[lastSize].imageData[i * 4 + 3] = 255;
					}
				}
				if (tex.sampler > -1) {
					// Set custom sampler
					OUT_LV_DEBUG("Set custom sampler " << tex.sampler)
						mesh->textures[lastSize].sampler = mesh->samplers[tex.sampler];
				}
				mesh->textures[lastSize].initTexture();
			}
		}

		// Loading materials
		static void loadMaterials(tinygltf::Model* model, gmc::Actor* mesh) {
			if (model->materials.size() < 1) {
				// TODO Make this default Material ...
				gmc::Material material = gmc::Material(glm::vec4(0, 0, 0, 1.0));
				material.createMaterial();
				mesh->materials.push_back(material);
				return;
			}
			for each (tinygltf::Material mat in model->materials) {
				OUT_LV_DEBUG("Load material " << mat.name)
				auto colorTex = mat.values.find("baseColorTexture");
				auto colorFactor = mat.values.find("baseColorFactor");

				auto doubleSided = mat.additionalValues.find("doubleSided");

				gmc::Material material;

				if (colorTex != mat.values.end()) {
					material = gmc::Material(&mesh->textures[colorTex->second.TextureIndex()],
						colorFactor != mat.values.end() ? glm::make_vec4(colorFactor->second.ColorFactor().data()) : glm::vec4(1));
				}
				else if (colorFactor != mat.values.end()) {
					gmc::Material material = gmc::Material(glm::make_vec4(colorFactor->second.ColorFactor().data()));
				}
				else {
					// TODO Make this default Material ...
					gmc::Material material = gmc::Material(glm::vec4(0, 0, 0, 1.0));
				}

				if (doubleSided != mat.additionalValues.end())
					material.doubleSided = doubleSided->second.bool_value;

				material.createMaterial();
				mesh->materials.push_back(material);
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
	const tinygltf::Accessor& name##Accessor = model->accessors[prim.attributes.find(cname)->second];\
	const tinygltf::BufferView& name##View = model->bufferViews[name##Accessor.bufferView];\
	name##Buffer = reinterpret_cast<const type*>(&(model->buffers[name##View.buffer].data[name##Accessor.byteOffset + name##View.byteOffset]));\
}

#define ADD_INDEX(type) {const type* buf = static_cast<const type*>(indexBuffer);\
for (size_t i = 0; i < indexAccessor.count; i++)\
{\
	mesh->indices.push_back((uint32_t) buf[i]);\
}}

		static void loadNodes(tinygltf::Model* model, gmc::Actor* actor) {
			actor->meshes.resize(model->nodes.size());
			actor->mats.resize(model->nodes.size());
			for (size_t i = 0; i < actor->meshes.size(); i++)
			{
				tge::gmc::Mesh* mesh = new tge::gmc::Mesh();
				tinygltf::Node node = model->nodes[i];
				if (node.mesh > -1) {
					const tinygltf::Mesh msh = model->meshes[model->nodes[i].mesh];
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

						const tinygltf::Accessor& posAccessor = model->accessors[prim.attributes.find("POSITION")->second];
						const tinygltf::BufferView& posView = model->bufferViews[posAccessor.bufferView];
						const float* posBuffer = reinterpret_cast<const float*>(&(model->buffers[posView.buffer].data[posAccessor.byteOffset + posView.byteOffset]));

						FIND(normal, float, "NORMAL")
							FIND(uv, float, "TEXCOORD_0")

							// TODO TANGENT? UV_1? JOINTS? WEIGHT? ...
							gmc::RenderOffsets offset;
						offset.material = prim.material < 0 ? 0 : prim.material;
						offset.offset = mesh->indices.size();

						if (prim.indices > -1) {
							for (size_t i = 0; i < posAccessor.count; i++)
							{
								TGVertex vert = {
									glm::make_vec3(&posBuffer[i * 3]),
									uvBuffer ? glm::make_vec2(&uvBuffer[i * 2]) : glm::vec2(0),
									normalBuffer ? glm::normalize(glm::make_vec3(&normalBuffer[i * 3])) : glm::vec3(0)
								};
								mesh->vertices.push_back(vert);
							}

							const tinygltf::Accessor& indexAccessor = model->accessors[prim.indices];
							const tinygltf::BufferView& indexView = model->bufferViews[indexAccessor.bufferView];
							const void* indexBuffer = &(model->buffers[indexView.buffer].data[indexAccessor.byteOffset + indexView.byteOffset]);

							offset.size = indexAccessor.count;

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
									glm::make_vec3(&posBuffer[i * 3]),
									uvBuffer ? glm::make_vec2(&uvBuffer[i * 2]) : glm::vec2(0),
									normalBuffer ? glm::normalize(glm::make_vec3(&normalBuffer[i * 3])) : glm::vec3(0)
								};
								mesh->add(vert);
							}
							offset.size = mesh->indices.size();
						}

						actor->offsets.push_back(offset);
					}
				}
				actor->mats[i] = glm::mat4(1);

				if (node.matrix.size() == 16) {
					actor->mats[i] = glm::make_mat4(node.matrix.data());
				}
				if (node.translation.size() == 3) {
					actor->mats[i] = glm::translate(actor->mats[i], glm::make_vec3((float*)node.translation.data()));
				}
				if (node.rotation.size() == 4) {
					actor->mats[i] = actor->mats[i] * glm::mat4_cast(glm::make_quat((float*)node.rotation.data()));
				}
				if (node.scale.size() == 3) {
					actor->mats[i] = glm::scale(actor->mats[i], glm::make_vec3((float*)node.scale.data()));
				}
				actor->meshes[i] = mesh;
			}

			for (size_t i = 0; i < model->nodes.size(); i++)
			{
				for (size_t j = 0; j < model->nodes[i].children.size(); j++)
				{
					actor->mats[model->nodes[i].children[j]] = actor->mats[i] * actor->mats[model->nodes[i].children[j]];
				}
			}
		}

		void loadGltf(char* name, gmc::Actor* mesh) {
			TinyGLTF loader;
			tinygltf::Model model;

			bool binary = strstr(name, ".glb");

			std::string err;
			std::string warn;

			// TODO use pak loading when avalaible
			bool check = binary ? loader.LoadBinaryFromFile(&model, &err, &warn, name) : loader.LoadASCIIFromFile(&model, &err, &warn, name);

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

			loadSampler(&model, mesh);
			loadTextures(&model, mesh);
			loadMaterials(&model, mesh);
			loadNodes(&model, mesh);

			if (mesh->meshes.size() < 1 || mesh->meshes[0]->vertices.size() < 1)
				return;
			// AABB Calculation
			tge::gmc::AABB aabb = { mesh->meshes[0]->vertices[0].position, mesh->meshes[0]->vertices[0].position };

			for (size_t j = 0; j < mesh->meshes.size(); j++)
			{
				for (size_t i = 0; i < mesh->meshes[j]->vertices.size(); i++)
				{
					glm::vec3 vert = mesh->meshes[j]->vertices[i].position;
					aabb.max.y = TGE_MAX(aabb.max.y, vert.y);
					aabb.max.z = TGE_MAX(aabb.max.z, vert.z);
					aabb.max.x = TGE_MAX(aabb.max.x, vert.x);

					aabb.min.x = TGE_MIN(aabb.min.x, vert.x);
					aabb.min.y = TGE_MIN(aabb.min.y, vert.y);
					aabb.min.z = TGE_MIN(aabb.min.z, vert.z);
				}
			}
			mesh->aabb = aabb;
#ifdef DEBUG
			aabb.print();
#endif // DEBUG

			glm::vec3 offset = glm::vec3(0) - (((aabb.max - aabb.min) / 2.0f) + aabb.min);

			mesh->prePos(offset.x, offset.y, offset.z)->applyPretransform();
		}
	}
}