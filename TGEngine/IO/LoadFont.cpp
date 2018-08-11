#include "LoadFont.hpp"



void loadfont(Font* font) {
	FILE* file = fopen(font->path, "rb");
#ifdef DEBUG
	if (!file) {
		OUT_LV_DEBUG("File " << font->path << " can not be opened, this should not happen!")
		return;
	}
#endif // DEBUG

	fseek(file, 0, SEEK_END);
	long size_of_file = ftell(file);
	fseek(file, 0, SEEK_SET);
	unsigned char * temp_buffer = new unsigned char[size_of_file];
	font->texture.image_data = new stbi_uc[512 * 512];
	font->texture.width = 128;
	font->texture.height = 128;

	fread(temp_buffer, sizeof(unsigned char), size_of_file, file);

	int i = stbtt_BakeFontBitmap(temp_buffer, 0, 32, font->texture.image_data, 512, 512, 0, 256, font->cdata);
	OUT_LV_DEBUG(i)
	delete[] temp_buffer;

	createTexture(&font->texture);
}

void Font::drawString(TGVertex vert, char* text, VertexBuffer* buffer) {
	unsigned char* string = reinterpret_cast<unsigned char*>(text);

	for (; *string; ++string) {
		stbtt_aligned_quad quad;
		stbtt_GetBakedQuad(this->cdata, 512, 512, *string, &vert.position.x, &vert.position.y, &quad, 0);
		buffer->add({
			{ quad.x0, quad.y0 , vert.position.z},
			vert.color,
			{ quad.s0, quad.t0 },
			this->texture.index
			});
		buffer->add({
			{ quad.x1, quad.y0 , vert.position.z },
			vert.color,
			{ quad.s1, quad.t0 },
			this->texture.index
			});
		buffer->add({
			{ quad.x1, quad.y1 , vert.position.z },
			vert.color,
			{ quad.s1, quad.t1 },
			this->texture.index
			});
		buffer->add({
			{ quad.x1, quad.y1 , vert.position.z },
			vert.color,
			{ quad.s1, quad.t1 },
			this->texture.index
			});
		buffer->add({
			{ quad.x0, quad.y1 , vert.position.z },
			vert.color,
			{ quad.s0, quad.t1 },
			this->texture.index
			});
		buffer->add({
			{ quad.x0, quad.y0 , vert.position.z },
			vert.color,
			{ quad.s0, quad.t0 },
			this->texture.index
			});
	}
}