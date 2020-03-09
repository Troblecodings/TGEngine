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
			textureInputInfo->data = new uint8_t[16000000];
			textureInputInfo->x = 2000;
			textureInputInfo->y = 2000;
			textureInputInfo->comp = 4;
			uint8_t* filedata = tge::nio::readAll(name, &size);
			stbtt_BakeFontBitmap(filedata, 0, fontheight, textureInputInfo->data, 2000, 2000, 0, 256, charData);
		}
		void createStringActor(const char* inputString, stbtt_bakedchar* charData, tge::gmc::ActorInputInfo *actor) {
			size_t inputStringLength = strlen(inputString);
			float x = 0;
			float y = 0;
			stbtt_aligned_quad alignedQuad;

			actor->indexCount = inputStringLength * 6;
			actor->vertexCount = inputStringLength * 4;
			actor->pIndices = new uint32_t[actor->indexCount];
			actor->pVertices = (uint8_t*)new float[actor->vertexCount * 4];
			float* arr = (float*)actor->pVertices;

			for (size_t i = 0; i < inputStringLength; i++) {
				int characterNumber = (int)inputString[i];
				stbtt_GetBakedQuad(charData, 2000, 2000, characterNumber, &x, &y, &alignedQuad, 0);
				arr[i * 16] = alignedQuad.x0;
				arr[i * 16 + 1] = alignedQuad.y0;
				arr[i * 16 + 2] = alignedQuad.s0;
				arr[i * 16 + 3] = alignedQuad.t0;

				arr[i * 16 + 4] = alignedQuad.x1;
				arr[i * 16 + 5] = alignedQuad.y0;
				arr[i * 16 + 6] = alignedQuad.s1;
				arr[i * 16 + 7] = alignedQuad.t0;

				arr[i * 16 + 8] = alignedQuad.x1;
				arr[i * 16 + 9] = alignedQuad.y1;
				arr[i * 16 + 10] = alignedQuad.s1;
				arr[i * 16 + 11] = alignedQuad.t1;

				arr[i * 16 + 12] = alignedQuad.x0;
				arr[i * 16 + 13] = alignedQuad.y1;
				arr[i * 16 + 14] = alignedQuad.s0;
				arr[i * 16 + 15] = alignedQuad.t1;

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