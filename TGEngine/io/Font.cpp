#include "Font.hpp"
#include "..\gamecontent\Camera.hpp"

namespace tge {
	namespace fnt {

		Font::Font(char* path, uint32_t height) : height(height) {
			uint8_t* fileData = tge::nio::readAll(path);
			ASSERT_NONE_NULL_DB(fileData, "File data nullptr!", TG_ERR_DB_NULLPTR)

				uint8_t* tempbitmap = new uint8_t[(size_t)height * (size_t)4096 * (size_t)height];
			stbtt_BakeFontBitmap(fileData, 0, (float)height, tempbitmap, height * 64, height * 64, 0, 256, this->cdata);

			delete[] fileData;

			uint8_t* colorData = new uint8_t[(size_t)height * (size_t)16384 * (size_t)height];

			for(size_t i = 0; i < ((size_t)height * (size_t)4096 * (size_t)height); i++) {
				colorData[i * 4] = tempbitmap[i];
				colorData[i * 4 + 1] = tempbitmap[i];
				colorData[i * 4 + 2] = tempbitmap[i];
				colorData[i * 4 + 3] = tempbitmap[i];
			}
			delete[] tempbitmap;

			this->texture = new tex::Texture(height * 64, height * 64);
			this->texture->imageData = colorData;

			this->mat = gmc::UIMaterial(this->texture);
			TG_VECTOR_APPEND_NORMAL(gmc::materiallist, &this->mat)
				this->materialIndex = (uint32_t)lastSize;
		}

		void Font::drawString(glm::vec2 pos, char* text, VertexBuffer* buffer, IndexBuffer* ibuffer, glm::vec4 color) {
			pos.y /= 0.002f;
			pos.x /= gmc::multiplier * 0.002f;
			while(*text) {
				stbtt_aligned_quad quad;
				stbtt_GetBakedQuad(this->cdata, this->texture->getWidth(), this->texture->getHeight(), *text, &pos.x, &pos.y, &quad, 0);
				uint32_t idcount = (uint32_t)buffer->pointCount;
				quad.x0 *= gmc::multiplier * 0.002f;
				quad.x1 *= gmc::multiplier * 0.002f;
				quad.y0 *= 0.002f;
				quad.y1 *= 0.002f;
				buffer->add({ quad.x0, quad.y0 })->add({ quad.s0, quad.t0 })->add(color)->endVertex();
				buffer->add({ quad.x1, quad.y0 })->add({ quad.s1, quad.t0 })->add(color)->endVertex();
				buffer->add({ quad.x1, quad.y1 })->add({ quad.s1, quad.t1 })->add(color)->endVertex();
				buffer->add({ quad.x0, quad.y1 })->add({ quad.s0, quad.t1 })->add(color)->endVertex();

				ibuffer->addIndex(idcount);
				ibuffer->addIndex(idcount + 1);
				ibuffer->addIndex(idcount + 2);
				ibuffer->addIndex(idcount);
				ibuffer->addIndex(idcount + 2);
				ibuffer->addIndex(idcount + 3);
				text++;
			}
		}

		uint32_t Font::getMaterialIndex() {
			return this->materialIndex;
		}

		glm::vec2 Font::getExtent(char* chr) {
			glm::vec2 pos = glm::vec2(0, 0);
			while(*chr) {
				stbtt_aligned_quad quad;
				stbtt_GetBakedQuad(this->cdata, this->texture->getWidth(), this->texture->getHeight(), *chr, &pos.x, &pos.y, &quad, 0);
				chr++;
			}
			pos.y = this->height * 0.0015f;
			pos.x *= gmc::multiplier * 0.002f;
			return pos;
		}
	}
}