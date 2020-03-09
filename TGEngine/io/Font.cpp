#include "Font.hpp"
#include "../gamecontent/camera/Camera.hpp"
#include "Files.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"
#include "../gamecontent/Actor.hpp"
#include <stdio.h>
#include <string.h>

namespace tge {
	namespace fnt {

		void readFontfile(const char* name, const float fontheight, tge::tex::TextureInputInfo* textureInputInfo, stbtt_bakedchar* charData) {
			long size = 0;
			textureInputInfo->data = new uint8_t[FONT_TEXTURE_WIDTH * FONT_TEXTURE_HEIGHT * 4];
			textureInputInfo->x = FONT_TEXTURE_WIDTH;
			textureInputInfo->y = FONT_TEXTURE_HEIGHT;
			textureInputInfo->comp = 4;
			uint8_t* filedata = tge::nio::readAll(name, &size);
			stbtt_BakeFontBitmap(filedata, 0, fontheight, textureInputInfo->data, FONT_TEXTURE_WIDTH, FONT_TEXTURE_HEIGHT, 0, 256, charData);
		}

		void createStringActor(const char* inputString, const stbtt_bakedchar* charDataIn, tge::gmc::ActorInputInfo *actor) {
			size_t inputStringLength = strlen(inputString);
			float x = 0;
			float y = 0;
			stbtt_aligned_quad alignedQuad;

			actor->indexCount = inputStringLength * 6;
			actor->vertexCount = inputStringLength * 4;
			actor->pIndices = new uint32_t[actor->indexCount];
			actor->pVertices = (uint8_t*)new float[((uint32_t)actor->vertexCount) * 4];
			float* arr = (float*)actor->pVertices;

			float ipw = 1.0f / FONT_TEXTURE_WIDTH, iph = 1.0f / FONT_TEXTURE_HEIGHT;

			for (size_t i = 0; i < inputStringLength; i++) {
				int characterNumber = (int)inputString[i];

				const stbtt_bakedchar* charData = charDataIn + characterNumber;

				int round_x = STBTT_ifloor((x + charData->xoff) + 0.5f);
				int round_y = STBTT_ifloor((y + charData->yoff) + 0.5f);

				int y1 = round_y + charData->y1 - charData->y0;
				int x1 = round_x + charData->x1 - charData->x0;

				int s0 = charData->x0 * ipw;
				int t0 = charData->y0 * iph;
				int s1 = charData->x1 * ipw;
				int t1 = charData->y1 * iph;

				x += charData->xadvance;

				arr[i * 16] = round_x;
				arr[i * 16 + 1] = round_y;
				arr[i * 16 + 2] = s0;
				arr[i * 16 + 3] = t0;

				arr[i * 16 + 4] = x1;
				arr[i * 16 + 5] = round_y;
				arr[i * 16 + 6] = s1;
				arr[i * 16 + 7] = t0;

				arr[i * 16 + 8] = x1;
				arr[i * 16 + 9] = y1;
				arr[i * 16 + 10] = s1;
				arr[i * 16 + 11] = t1;

				arr[i * 16 + 12] = round_x;
				arr[i * 16 + 13] = y1;
				arr[i * 16 + 14] = s0;
				arr[i * 16 + 15] = t1;

				actor->pIndices[i * 6] = i * 4 + 0;
				actor->pIndices[i * 6 + 1] = i * 4 + 1;
				actor->pIndices[i * 6 + 2] = i * 4 + 2;

				actor->pIndices[i * 6 + 3] = i * 4 + 0;
				actor->pIndices[i * 6 + 4] = i * 4 + 2;
				actor->pIndices[i * 6 + 5] = i * 4 + 3;
			}
			
		}
	}
}