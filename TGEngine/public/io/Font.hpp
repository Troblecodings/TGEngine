#pragma once

#include <vector>
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include "../../stb/stb_truetype.h"
#include "../../public/pipeline/buffer/Memory.hpp"
#include "../../public/gamecontent/Actor.hpp"
#include "../../public/drawlib/Quad.hpp"
#include "../../public/pipeline/CommandBuffer.hpp"
#include "../../public/pipeline/window/Window.hpp"

namespace tge::fnt {

		constexpr uint32_t FONT_TEXTURE_WIDTH = 2000;
		constexpr uint32_t FONT_TEXTURE_HEIGHT = 2000;

		struct Font {
			uint32_t material;
			stbtt_bakedchar* charData;
			float fontheight; // Primarily for alignment
		};

		extern std::vector<tge::fnt::Font> fonts;
		extern std::vector<tge::buf::BufferObject> fontBufferObjects;

		constexpr float homogenHeight(const Font* pFont) {
			return pFont->fontheight / tge::win::mainWindowHeight;
		}

		template<typename T>
		inline const uint32_t createStringActor(const Font* pFont, const T* pInputStrings, const uint32_t size, const glm::mat4* tranforms) {
			constexpr bool isValid = std::is_same_v<T, const char*> || std::is_same_v<T, std::string>;
			static_assert(isValid, "Only Strings and C-Strings allowed");

			constexpr float ipw = 1.0f / FONT_TEXTURE_WIDTH, iph = 1.0f / FONT_TEXTURE_HEIGHT;

			uint32_t vertexLength = 0;
			uint32_t indexLength = 0;

			const uint32_t cindex = fontBufferObjects.size();
			const uint32_t cglobalindex = cindex + LAYER_ID_OFFSET;
			fontBufferObjects.resize(cglobalindex);

			const uint32_t reservesize = tge::gmc::actorProperties.size() + size;
			tge::gmc::actorProperties.reserve(reservesize);
			tge::gmc::actorDescriptor.reserve(reservesize);

			tge::gmc::ActorProperties properties;
			properties.transform.matrix = glm::mat4(1);
			properties.transform.animationIndex = 0;
			properties.transform.transformIndex = 0;
			properties.material = pFont->material;
			properties.layer = cglobalindex;

			for (size_t i = 0; i < size; i++) {
				uint32_t inputStringLength = 0;
				if constexpr (std::is_same_v<T, std::string>) {
					inputStringLength = pInputStrings[i].size();
				} else {
					const char* cstring = pInputStrings[i];
					inputStringLength = (uint32_t)strlen(cstring);
				}
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
			const uint32_t vertexByteSize = vertexLength * 4 * sizeof(float);
			const uint32_t indexByteSize = indexLength * sizeof(uint32_t);

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

			const float FONT_MULTIPLIER = 1.0f / (0.6f * (float)tge::win::mainWindowHeight);

			uint32_t quadCount = 0;
			for (uint32_t i = 0; i < size; i++) {
				const char* cstring;
				if constexpr (std::is_same_v<T, std::string>) {
					cstring = pInputStrings[i].c_str();
				} else {
					cstring = pInputStrings[i];
				}

				float x = 0;

				uint32_t cquat = 0;
				while (*cstring) {
					const int i = *cstring;

#ifdef DEBUG
					if (i >= 128) {
						OUT_LV_DEBUG(i << " is not a valid char!")
					}
#endif // DEBUG

					const stbtt_bakedchar* charData = pFont->charData + i;

					const float round_x = STBTT_ifloor((x + charData->xoff) + 0.5f) * FONT_MULTIPLIER;
					const float round_y = STBTT_ifloor(charData->yoff + 0.5f) * FONT_MULTIPLIER;
					const float y1 = round_y + (charData->y1 - charData->y0) * FONT_MULTIPLIER;
					const float x1 = round_x + (charData->x1 - charData->x0) * FONT_MULTIPLIER;

					const float s0 = charData->x0 * ipw;
					const float t0 = charData->y0 * iph;
					const float s1 = charData->x1 * ipw;
					const float t1 = charData->y1 * iph;

					x += charData->xadvance;

					const uint8_t* vert = (uint8_t*)vertexMemory + quadCount * 16u * sizeof(float);
					const uint8_t* indx = (uint8_t*)indexMemory + quadCount * 6u * sizeof(uint32_t);

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
			return cglobalindex;
		}

		void destroyFontresources();

		void destroyStrings(const uint32_t destroy);
}
