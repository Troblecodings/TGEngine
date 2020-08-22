#include "Resource.hpp"
#include "../io/Files.hpp"
#include "../io/Font.hpp"
#include "../gamecontent/Actor.hpp"
#include "../pipeline/CommandBuffer.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace tge::io {

#ifdef DEBUG
#define SIZE_CHECK if (blocklength != UINT32_MAX) {\
		OUT_LV_DEBUG("Something went wrong while reading, out of size! L " << __LINE__)\
	}
#else
#define SIZE_CHECK
#endif // DEBUG

	using namespace tge::nio;
	using namespace tge::tex;
	using namespace tge::gmc;
	using namespace tge::fnt;
	using namespace tge::buf;

	Map currentMap;

	inline static void loadActorV2(File file) noexcept {
		uint32_t actorCount = 0;
		fread(&actorCount, sizeof(uint32_t), 1, file);

		// Start to read the actor
		actorProperties.resize(actorCount);
		actorDescriptor.resize(actorCount);
		actorInstanceDescriptor.reserve(actorCount);

		uint32_t blocklength;
		fread(&blocklength, sizeof(uint32_t), 1, file);

		uint32_t bufferCount = actorCount * 2;

		BufferObject* buffer = new BufferObject[bufferCount];
		VkBufferCopy* indexBufferCopy = new VkBufferCopy[actorCount];
		VkBufferCopy* vertexBufferCopy = new VkBufferCopy[actorCount];

		uint32_t lastVertexOffset = 0;
		uint32_t lastIndexOffset = 0;

		uint32_t lastVertexCount = 0;
		uint32_t lastIndexCount = 0;

		// Goes on until it hits a change request block size (2^32)
		for (uint32_t currentId = 0; currentId < actorCount; currentId++) {

			// Reads the actor properties and so on
			ActorProperties* currentProperty = actorProperties.data() + currentId;
			ActorDescriptor* currentDescription = actorDescriptor.data() + currentId;

			fread(currentProperty, sizeof(ActorProperties), 1, file);

			ActorInstanceDescriptor currentInstanceDescription;
			fread(&currentInstanceDescription, sizeof(ActorInstanceDescriptor), 1, file);
			if (currentInstanceDescription.instanceCount > 1) {
				currentDescription->instanceID = actorInstanceDescriptor.size();
				actorInstanceDescriptor.push_back(currentInstanceDescription);
			} else {
				currentDescription->instanceID = UINT32_MAX;
			}

			uint32_t vertexCount = 0;
			fread(&currentDescription->indexDrawCount, sizeof(uint32_t), 1, file);
			fread(&vertexCount, sizeof(uint32_t), 1, file);

			currentDescription->indexOffset = lastIndexCount;
			currentDescription->vertexOffset = lastVertexCount;
			lastIndexCount += currentDescription->indexDrawCount;
			lastVertexCount += vertexCount;

			indexBufferCopy[currentId].dstOffset = lastIndexOffset;
			indexBufferCopy[currentId].srcOffset = 0;
			vertexBufferCopy[currentId].dstOffset = lastVertexOffset;
			vertexBufferCopy[currentId].srcOffset = 0;

			// Creating staging buffer for indices and vertices
			BufferInputInfo bufferInputInfos[2];
			bufferInputInfos[0].flags = VK_SHADER_STAGE_ALL_GRAPHICS;
			bufferInputInfos[0].size = sizeof(uint32_t) * currentDescription->indexDrawCount;
			bufferInputInfos[0].memoryIndex = vlibDeviceHostVisibleCoherentIndex;
			bufferInputInfos[0].bufferUsageFlag = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

			bufferInputInfos[1].flags = VK_SHADER_STAGE_ALL_GRAPHICS;
			bufferInputInfos[1].size = blocklength;
			bufferInputInfos[1].memoryIndex = vlibDeviceHostVisibleCoherentIndex;
			bufferInputInfos[1].bufferUsageFlag = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

			lastIndexOffset += (uint32_t)(indexBufferCopy[currentId].size = bufferInputInfos[0].size);
			lastVertexOffset += (uint32_t)(vertexBufferCopy[currentId].size = blocklength);

			createBuffers(bufferInputInfos, 2, &buffer[currentId * 2]);

			void* indexMemory;
			CHECKFAIL(vkMapMemory(device, buffer[currentId * 2].memory, 0, VK_WHOLE_SIZE, 0, &indexMemory));

			// Reads the indices from the file
			fread(indexMemory, sizeof(uint32_t), currentDescription->indexDrawCount, file);

			vkUnmapMemory(device, buffer[currentId * 2].memory);

			void* vertexMemory;
			CHECKFAIL(vkMapMemory(device, buffer[currentId * 2 + 1].memory, 0, VK_WHOLE_SIZE, 0, &vertexMemory));

			// Reads the vertices
			fread(vertexMemory, sizeof(uint8_t), blocklength, file);

			vkUnmapMemory(device, buffer[currentId * 2 + 1].memory);

			// Read the next block size
			fread(&blocklength, sizeof(uint32_t), 1, file);
		}

#ifdef DEBUG
		if (lastIndexOffset == 0 || lastIndexCount == 0) {
			OUT_LV_DEBUG("It seems like you are missing indices!")
		}
		if (lastVertexOffset == 0 || lastVertexCount == 0) {
			OUT_LV_DEBUG("It seems like you are missing vertices!")
		}
#endif // DEBUG


		BufferInputInfo bufferInputInfos[TGE_MAP_BUFFER_COUNT];
		bufferInputInfos[0].flags = VK_SHADER_STAGE_ALL_GRAPHICS;
		bufferInputInfos[0].size = lastIndexOffset;
		bufferInputInfos[0].memoryIndex = vlibDeviceLocalMemoryIndex;
		bufferInputInfos[0].bufferUsageFlag = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		bufferInputInfos[1].flags = VK_SHADER_STAGE_ALL_GRAPHICS;
		bufferInputInfos[1].size = lastVertexOffset;
		bufferInputInfos[1].memoryIndex = vlibDeviceLocalMemoryIndex;
		bufferInputInfos[1].bufferUsageFlag = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		bufferInputInfos[2].flags = VK_SHADER_STAGE_ALL_GRAPHICS;
		bufferInputInfos[2].size = blocklength * INSTANCE_LENGTH + sizeof(glm::vec4);
		bufferInputInfos[2].memoryIndex = vlibDeviceLocalMemoryIndex;
		bufferInputInfos[2].bufferUsageFlag = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		bufferInputInfos[3].flags = VK_SHADER_STAGE_ALL_GRAPHICS;
		bufferInputInfos[3].size = blocklength * INSTANCE_LENGTH;
		bufferInputInfos[3].memoryIndex = vlibDeviceHostVisibleCoherentIndex;
		bufferInputInfos[3].bufferUsageFlag = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		createBuffers(bufferInputInfos, TGE_MAP_BUFFER_COUNT, currentMap.mapBuffers);

		startSingleTimeCommand();

		for (uint32_t i = 0; i < actorCount; i++) {
			vkCmdCopyBuffer(SINGLE_TIME_COMMAND_BUFFER, buffer[i * 2].buffer, currentMap.mapBuffers[0].buffer, 1, indexBufferCopy + i);
			vkCmdCopyBuffer(SINGLE_TIME_COMMAND_BUFFER, buffer[i * 2 + 1].buffer, currentMap.mapBuffers[1].buffer, 1, vertexBufferCopy + i);
		}

		endSingleTimeCommand();

		destroyBuffers(buffer, bufferCount);
		delete[] buffer;
		delete[] indexBufferCopy;
		delete[] vertexBufferCopy;
	}

	inline static void loadTexturesV2(File file, std::vector<TextureInputInfo>& textureInputInfos) noexcept {
		uint32_t textureCount;
		fread(&textureCount, sizeof(uint32_t), 1, file);
		textureInputInfos.resize(textureCount);

		uint32_t blocklength = 0;
		fread(&blocklength, sizeof(uint32_t), 1, file);

		for (uint32_t id = 0; id < textureCount; id++) {
			stbi_uc* resbuffer = new stbi_uc[blocklength];
			fread(resbuffer, sizeof(stbi_uc), blocklength, file);

			TextureInputInfo* inputInfo = textureInputInfos.data() + id;
			inputInfo->data = stbi_load_from_memory(resbuffer, (int)blocklength, &inputInfo->x, &inputInfo->y, &inputInfo->comp, STBI_rgb_alpha);

			fread(&blocklength, sizeof(uint32_t), 1, file);

			delete[] resbuffer;
		}
		SIZE_CHECK;
	}

	inline static uint32_t loadMaterialsV2(File file) {
		uint32_t materialCount = 0;
		fread(&materialCount, sizeof(uint32_t), 1, file);
		createdMaterials = new Material[255];

		uint32_t blocklength = 0;
		fread(&blocklength, sizeof(uint32_t), 1, file);

		for (size_t i = 0; i < materialCount; i++) {
#ifdef DEBUG
			if (blocklength != sizeof(Material)) {
				OUT_LV_DEBUG("The materials are out of size! Must be " << sizeof(Material) << ", but is " << blocklength)
			}
#endif // DEBUG

			fread(createdMaterials + i, blocklength, 1, file);
			fread(&blocklength, sizeof(uint32_t), 1, file);
		}

		SIZE_CHECK;
		return materialCount;
	}

	inline static void loadFontsV2(File file, std::vector<TextureInputInfo>& textureInputInfos, uint32_t materialCount) {
		uint32_t fontCount = 0;
		// Reads the count of blocks following
		fread(&fontCount, sizeof(uint32_t), 1, file);
		fonts.resize(fontCount);

		uint32_t textureCount = textureInputInfos.size();
		textureInputInfos.resize(textureCount + fontCount);

		// Read the block size of the following content
		uint32_t blocklength = 0;
		fread(&blocklength, sizeof(uint32_t), 1, file);

		for (uint32_t i = 0; i < fontCount; i++) {
			Font* font = fonts.data() + i;
			fread(&font->fontheight, sizeof(float), 1, file);

			uint8_t* resbuffer = new uint8_t[blocklength];
			fread(resbuffer, sizeof(uint8_t), blocklength, file);

			TextureInputInfo* textureInputInfo = textureInputInfos.data() + textureCount + i;
			textureInputInfo->data = new uint8_t[FONT_TEXTURE_WIDTH * FONT_TEXTURE_HEIGHT * 4];
			textureInputInfo->x = FONT_TEXTURE_WIDTH;
			textureInputInfo->y = FONT_TEXTURE_HEIGHT;
			textureInputInfo->comp = 4;

			font->charData = new stbtt_bakedchar[256];
			uint8_t* interdata = new uint8_t[FONT_TEXTURE_WIDTH * FONT_TEXTURE_HEIGHT];
			stbtt_BakeFontBitmap(resbuffer, 0, font->fontheight, interdata, FONT_TEXTURE_WIDTH, FONT_TEXTURE_HEIGHT, 0, 128, font->charData);
			delete[] resbuffer;

			for (size_t i = 0; i < FONT_TEXTURE_WIDTH * FONT_TEXTURE_HEIGHT; i++) {
				textureInputInfo->data[i * 4] = interdata[i];
				textureInputInfo->data[i * 4 + 1] = interdata[i];
				textureInputInfo->data[i * 4 + 2] = interdata[i];
				textureInputInfo->data[i * 4 + 3] = interdata[i];
			}
			delete[] interdata;

			font->material = i + materialCount;
			Material* material = createdMaterials + font->material;
			material->color = glm::vec4(1);
			material->diffuseTexture = textureCount + i;

			fread(&blocklength, sizeof(uint32_t), 1, file);
		}
	}

	inline static void loadTransformsV2(File file) noexcept {
		uint32_t sizeOfFloats = 0;
		fread(&sizeOfFloats, sizeof(uint32_t), 1, file);

		currentMap.transforms.resize(sizeOfFloats / 4);
		fread(currentMap.transforms.data(), sizeof(float), sizeOfFloats, file);

		uint32_t blocklength = 0;
		fread(&blocklength, sizeof(uint32_t), 1, file);
		SIZE_CHECK;
	}

	inline static void loadInstancesV2(File file) {
		uint32_t instanceCount = 0;
		fread(&instanceCount, sizeof(uint32_t), 1, file);

		void* memory;
		CHECKFAIL(vkMapMemory(device, currentMap.mapBuffers[3].memory, 0, VK_WHOLE_SIZE, 0, &memory));

		fread(memory, INSTANCE_LENGTH, instanceCount, file);

		vkUnmapMemory(device, currentMap.mapBuffers[3].memory);

		startSingleTimeCommand();

		VkBufferCopy bufferCopy;
		bufferCopy.srcOffset = 0;
		bufferCopy.dstOffset = sizeof(glm::vec4);
		bufferCopy.size = instanceCount * INSTANCE_LENGTH;
		vkCmdCopyBuffer(SINGLE_TIME_COMMAND_BUFFER, currentMap.mapBuffers[3].buffer, currentMap.mapBuffers[2].buffer, 1, &bufferCopy);

		endSingleTimeCommand();

		destroyBuffers(&currentMap.mapBuffers[3], 1);

		uint32_t blocklength = 0;
		fread(&blocklength, sizeof(uint32_t), 1, file);
		SIZE_CHECK;
	}

	inline static void loadResourceFileV2(File file) noexcept {
		std::vector<TextureInputInfo> textureInputInfos;

		loadTexturesV2(file, textureInputInfos);

		uint32_t materialCount = loadMaterialsV2(file);

		loadActorV2(file);

		loadInstancesV2(file);

		loadTransformsV2(file);

		loadFontsV2(file, textureInputInfos, materialCount);

		fclose(file);

		SamplerInputInfo inputInfo;
		inputInfo.uSamplerMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		inputInfo.vSamplerMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		inputInfo.filterMagnification = VK_FILTER_NEAREST;
		inputInfo.filterMignification = VK_FILTER_NEAREST;

		createSampler(inputInfo, &currentMap.sampler);

		currentMap.textures.resize(textureInputInfos.size());
		createTextures(textureInputInfos.data(), (uint32_t)textureInputInfos.size(), currentMap.textures.data());
		return;
	}

	void loadResourceFile(const char* name) noexcept {
		File file = open(name, "rb");

		uint32_t header = 0;
		fread(&header, sizeof(uint32_t), 1, file);

#ifdef DEBUG
		if (header != TGR_LATEST) {
			OUT_LV_DEBUG("Warning: you are using an old version of the tgr file, consider updating!");
		}
#endif // DEBUG

		if (header == TGR_VERSION_1) {
			OUT_LV_DEBUG("Support for version 1 was removed, skipping!");
		} else if (header == TGR_VERSION_2) {
			loadResourceFileV2(file);
		} else {
			OUT_LV_DEBUG("Header does not match with the parser versions, skipping!");
			return;
		}
	}

	void destroyResource() noexcept {
		destroyTexture(currentMap.textures.data(), currentMap.textures.size());
		destroySampler(currentMap.sampler);
		destroyBuffers(currentMap.mapBuffers, TGE_MAP_BUFFER_COUNT - 1);
	}
}
