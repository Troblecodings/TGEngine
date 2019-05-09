#include "Font.hpp"

namespace tg_font {

	Font::Font(char* path, uint32_t height) : height(height){
		stbi_uc* colorData = new stbi_uc[height * 16384 * (size_t)height];

		stbi_uc* tempbitmap = new stbi_uc[height * 4096 * (size_t)height];
		stbtt_BakeFontBitmap(tge::nio::readAll(path), 0, height, tempbitmap, height * 64, height * 64, 0, 256, this->cdata);

		for (size_t i = 0; i < (height * 128 * (size_t)height); i++)
		{
			colorData[i * 4] = tempbitmap[i];
			colorData[i * 4 + 1] = tempbitmap[i];
			colorData[i * 4 + 2] = tempbitmap[i];
			colorData[i * 4 + 3] = tempbitmap[i];
		}
		delete[] tempbitmap;
		this->texture = Texture(colorData, height * 64, height * 64);

		Material mat;
		mat.texture = &this->texture;
		mat.isUI = true;
		mat.color = glm::vec4(1, 1, 1, 1);
		TG_VECTOR_APPEND_NORMAL(materials, mat)
		this->material = last_size;
	}

	void Font::drawString(glm::vec2 pos, char* text, VertexBuffer* buffer, IndexBuffer* ibuffer) {
		pos.y /= 0.002;
		pos.x /= multiplier * 0.002;
		while (*text)
		{
			stbtt_aligned_quad quad;
			stbtt_GetBakedQuad(this->cdata, this->texture.getWidth(), this->texture.getHeight(), *text, &pos.x, &pos.y, &quad, 0);
			uint32_t idcount = (uint32_t)buffer->count_of_points;
			quad.x0 *= multiplier * 0.002;
			quad.x1 *= multiplier * 0.002;
			quad.y0 *= 0.002;
			quad.y1 *= 0.002;
			buffer->add({
				{ quad.x0, quad.y0, 0.1 },
				{ quad.s0, quad.t0 },
				});
			buffer->add({
				{ quad.x1, quad.y0, 0.1 },
				{ quad.s1, quad.t0 },
				});
			buffer->add({
				{ quad.x1, quad.y1, 0.1 },
				{ quad.s1, quad.t1 },
				});
			buffer->add({
				{ quad.x0, quad.y1, 0.1 },
				{ quad.s0, quad.t1 },
				});
			ibuffer->addIndex(idcount);
			ibuffer->addIndex(idcount + 1);
			ibuffer->addIndex(idcount + 2);
			ibuffer->addIndex(idcount);
			ibuffer->addIndex(idcount + 2);
			ibuffer->addIndex(idcount + 3);
			text++;
		}
	}

	uint32_t Font::getMaterialIndex()
	{
		return this->material;
	}

	glm::vec2 Font::getExtent(char* chr) {
		glm::vec2 pos = glm::vec2(0, 0);
		while (*chr)
		{
			stbtt_aligned_quad quad;
			stbtt_GetBakedQuad(this->cdata, this->texture.getWidth(), this->texture.getHeight(), *chr, &pos.x, &pos.y, &quad, 0);
			chr++;
		}
		pos.y = this->height * 0.0015;
		pos.x *= multiplier * 0.002;
		return pos;
	}

}