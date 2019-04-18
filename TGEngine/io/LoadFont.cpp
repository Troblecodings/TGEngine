#include "LoadFont.hpp"

namespace tg_font {

	Font::Font(char* path, int height) {
		this->texture.texture_path = nullptr;
		this->texture.width = height;
		this->texture.height = height * 255;
		this->texture.image_data = new stbi_uc[this->texture.width * (size_t)this->texture.height * 4];

		stbi_uc* tempbitmap = new stbi_uc[this->texture.width * (size_t)this->texture.height];

		stbtt_BakeFontBitmap(nio::readAll(path), 0, height, tempbitmap, this->texture.width, this->texture.height, 0, 256, this->cdata);

		for (size_t i = 0; i < (this->texture.width * (size_t)this->texture.height); i++)
		{
			this->texture.image_data[i * 4] = tempbitmap[i];
			this->texture.image_data[i * 4 + 1] = tempbitmap[i];
			this->texture.image_data[i * 4 + 2] = tempbitmap[i];
			this->texture.image_data[i * 4 + 3] = tempbitmap[i];
		}
		delete[] tempbitmap;
		createTexture(&this->texture);

		Material mat;
		mat.texture = &this->texture;
		mat.isUI = true;
		mat.color = glm::vec4(1, 1, 1, 1);
		TG_VECTOR_APPEND_NORMAL(materials, mat)
		this->material = last_size;
	}

	float Font::drawString(glm::vec3 pos, glm::vec4 color, char* text, VertexBuffer* buffer, IndexBuffer* ibuffer, float multi) {
		pos.x /= multi;
		pos.y /= multi;
		float width = 0;

		RenderOffsets off;
		off.material = this->material;
		off.offset = ibuffer->index_count;
		while (*text)
		{
			stbtt_aligned_quad quad;
			stbtt_GetBakedQuad(this->cdata, this->texture.width, this->texture.height, *text, &pos.x, &pos.y, &quad, 0);
			uint32_t idcount = (uint32_t)buffer->count_of_points;
			buffer->add({
				{ quad.x0 * multi, quad.y0 * multi, pos.z},
				{ quad.s0, quad.t0 },
				});
			buffer->add({
				{ quad.x1 * multi, quad.y0 * multi, pos.z },
				{ quad.s1, quad.t0 },
				});
			buffer->add({
				{ quad.x1 * multi, quad.y1 * multi, pos.z },
				{ quad.s1, quad.t1 },
				});
			buffer->add({
				{ quad.x0 * multi, quad.y1 * multi, pos.z },
				{ quad.s0, quad.t1 },
				});
			ibuffer->addIndex(idcount);
			ibuffer->addIndex(idcount + 1);
			ibuffer->addIndex(idcount + 2);
			ibuffer->addIndex(idcount);
			ibuffer->addIndex(idcount + 2);
			ibuffer->addIndex(idcount + 3);
			text++;
			if (text == nullptr) {
				width = pos.x;
			}
		}
		off.size = ibuffer->index_count - off.offset;
		render_offset.push_back(off);
		return width;
	}

}