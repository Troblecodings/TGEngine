#include "Font.hpp"

#include "../drawlib/Quad.hpp"
#include "../pipeline/CommandBuffer.hpp"
#include "../gamecontent/Actor.hpp"

namespace tge::fnt {

	std::vector<tge::fnt::Font> fonts;
	std::vector<tge::buf::BufferObject> fontBufferObjects;

	void createStringActor(Font* pFont, const char** pInputStrings, uint32_t size, glm::mat4* tranforms) {
		float ipw = 1.0f / FONT_TEXTURE_WIDTH, iph = 1.0f / FONT_TEXTURE_HEIGHT;

		uint32_t vertexByteSize = 0;
		uint32_t indexByteSize = 0;
		uint32_t vertexLength = 0;
		uint32_t indexLength = 0;

		uint32_t cindex = fontBufferObjects.size();
		fontBufferObjects.resize(cindex + 4);

		tge::gmc::actorProperties.reserve(tge::gmc::actorProperties.size() + size);
		tge::gmc::actorDescriptor.reserve(tge::gmc::actorDescriptor.size() + size);

		tge::gmc::ActorProperties properties;
		properties.transform.matrix = glm::mat4(1);
		properties.transform.animationIndex = 0;
		properties.transform.transformIndex = 0;
		properties.material = pFont->material;
		properties.layer = cindex + 4;

		for (size_t i = 0; i < size; i++) {
			const char* cstring = pInputStrings[i];
			uint32_t inputStringLength = (uint32_t)strlen(cstring);
			properties.transform.matrix = tranforms[i];
			tge::gmc::actorProperties.push_back(properties);

			tge::gmc::ActorDescriptor descriptor;
			descriptor.indexDrawCount = 6 * inputStringLength;
			descriptor.indexOffset = indexLength;
			descriptor.vertexOffset = vertexLength;
			descriptor.instanceID = UINT32_MAX;
			tge::gmc::actorDescriptor.push_back(descriptor);

			indexLength += 6 * inputStringLength;
			vertexLength += 4 * inputStringLength;
		}
		vertexByteSize = vertexLength * 4 * sizeof(float);
		indexByteSize = indexLength * sizeof(uint32_t);

		tge::buf::BufferInputInfo bufferInfos[4];
		bufferInfos[0].flags = VK_SHADER_STAGE_ALL_GRAPHICS;
		bufferInfos[0].size = indexByteSize;
		bufferInfos[0].memoryIndex = vlibDeviceLocalMemoryIndex;
		bufferInfos[0].bufferUsageFlag = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

		bufferInfos[1].flags = VK_SHADER_STAGE_ALL_GRAPHICS;
		bufferInfos[1].size = vertexByteSize;
		bufferInfos[1].memoryIndex = vlibDeviceLocalMemoryIndex;
		bufferInfos[1].bufferUsageFlag = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

		bufferInfos[2].flags = VK_SHADER_STAGE_ALL_GRAPHICS;
		bufferInfos[2].size = indexByteSize;
		bufferInfos[2].memoryIndex = vlibDeviceHostVisibleCoherentIndex;
		bufferInfos[2].bufferUsageFlag = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

		bufferInfos[3].flags = VK_SHADER_STAGE_ALL_GRAPHICS;
		bufferInfos[3].size = vertexByteSize;
		bufferInfos[3].memoryIndex = vlibDeviceHostVisibleCoherentIndex;
		bufferInfos[3].bufferUsageFlag = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

		tge::buf::BufferObject* bufferStorage = fontBufferObjects.data() + cindex;

		tge::buf::createBuffers(bufferInfos, 4, bufferStorage);

		void* indexMemory;
		CHECKFAIL(vkMapMemory(device, bufferStorage[2].memory, 0, indexByteSize, 0, &indexMemory));

		void* vertexMemory;
		CHECKFAIL(vkMapMemory(device, bufferStorage[3].memory, 0, vertexByteSize, 0, &vertexMemory));

		uint32_t quadCount = 0;
		for (uint32_t i = 0; i < size; i++) {
			const char* cstring = pInputStrings[i];

			float x = 0;
			float y = 0;

			uint32_t cquat = 0;
			while (*cstring) {
				int i = *cstring;

#ifdef DEBUG
				if (i >= 128) {
					OUT_LV_DEBUG(i << " is not a valid char!")
				}
#endif // DEBUG

				const stbtt_bakedchar* charData = pFont->charData + i;


				float round_x = STBTT_ifloor((x + charData->xoff) + 0.5f) * FONT_MULTIPLIER;
				float round_y = STBTT_ifloor((y + charData->yoff) + 0.5f) * FONT_MULTIPLIER;
				float y1 = round_y + (charData->y1 - charData->y0) * FONT_MULTIPLIER;
				float x1 = round_x + (charData->x1 - charData->x0) * FONT_MULTIPLIER;

				float s0 = charData->x0 * ipw;
				float t0 = charData->y0 * iph;
				float s1 = charData->x1 * ipw;
				float t1 = charData->y1 * iph;

				x += charData->xadvance;

				uint8_t* vert = (uint8_t*)vertexMemory + quadCount * 16u * sizeof(float);
				uint8_t* indx = (uint8_t*)indexMemory + quadCount * 6u * sizeof(uint32_t);

				tge::drw::genQuad(x1, y1, round_x, round_y, s1, t1, s0, t0, vert, indx, cquat * 4);
				quadCount++;
				cstring++;
				cquat++;
			}

		}

		vkUnmapMemory(device, bufferStorage[2].memory);
		vkUnmapMemory(device, bufferStorage[3].memory);

		startSingleTimeCommand();

		VkBufferCopy indexBufferCopy;
		indexBufferCopy.srcOffset = 0;
		indexBufferCopy.dstOffset = 0;
		indexBufferCopy.size = indexByteSize;
		vkCmdCopyBuffer(SINGLE_TIME_COMMAND_BUFFER, bufferStorage[2].buffer, bufferStorage[0].buffer, 1, &indexBufferCopy);

		VkBufferCopy vertexBufferCopy;
		vertexBufferCopy.srcOffset = 0;
		vertexBufferCopy.dstOffset = 0;
		vertexBufferCopy.size = vertexByteSize;
		vkCmdCopyBuffer(SINGLE_TIME_COMMAND_BUFFER, bufferStorage[3].buffer, bufferStorage[1].buffer, 1, &vertexBufferCopy);

		endSingleTimeCommand();

		tge::buf::destroyBuffers(bufferStorage + 2, 2);

		fontBufferObjects.erase(fontBufferObjects.end() - 2, fontBufferObjects.end());
		fontBufferObjects.shrink_to_fit();
	}

	void destroyFontresources() {
		tge::buf::destroyBuffers(fontBufferObjects.data(), fontBufferObjects.size());
	}
}