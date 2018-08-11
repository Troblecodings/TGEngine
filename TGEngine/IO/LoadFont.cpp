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
	font->texture.width = 512;
	font->texture.height = 512;
	font->texture.image_data = new stbi_uc[font->texture.width * font->texture.height * 4];

	stbi_uc* tempbitmap = new stbi_uc[font->texture.width * font->texture.height];

	fread(temp_buffer, sizeof(unsigned char), size_of_file, file);

	stbtt_BakeFontBitmap(temp_buffer, 0, 32, tempbitmap, font->texture.width, font->texture.height, 0, 256, font->cdata);
	delete[] temp_buffer;

	for (size_t i = 0; i < font->texture.width * font->texture.height; i++)
	{
		font->texture.image_data[i * 4] = tempbitmap[i];
		font->texture.image_data[i * 4 + 1] = tempbitmap[i];
		font->texture.image_data[i * 4 + 2] = tempbitmap[i];
		font->texture.image_data[i * 4 + 3] = tempbitmap[i];
	}
	delete[] tempbitmap;
	createTexture(&font->texture);
}

void Font::drawString(TGVertex vert, char* text, VertexBuffer* buffer) {
	unsigned char* string = reinterpret_cast<unsigned char*>(text);

	for (; *string; ++string) {
		stbtt_aligned_quad quad;
		stbtt_GetBakedQuad(this->cdata, this->texture.width, this->texture.height, *string, &vert.position.x, &vert.position.y, &quad, 0);
		buffer->add({
			{ quad.x0 MULTIPLAIER, quad.y0 MULTIPLAIER, vert.position.z},
			vert.color,
			{ quad.s0, quad.t0 },
			this->texture.index
			});
		buffer->add({
			{ quad.x1 MULTIPLAIER, quad.y0 MULTIPLAIER, vert.position.z },
			vert.color,
			{ quad.s1, quad.t0 },
			this->texture.index
			});
		buffer->add({
			{ quad.x1 MULTIPLAIER, quad.y1 MULTIPLAIER, vert.position.z },
			vert.color,
			{ quad.s1, quad.t1 },
			this->texture.index
			});
		buffer->add({
			{ quad.x1 MULTIPLAIER, quad.y1 MULTIPLAIER, vert.position.z },
			vert.color,
			{ quad.s1, quad.t1 },
			this->texture.index
			});
		buffer->add({
			{ quad.x0 MULTIPLAIER, quad.y1 MULTIPLAIER, vert.position.z },
			vert.color,
			{ quad.s0, quad.t1 },
			this->texture.index
			});
		buffer->add({
			{ quad.x0 MULTIPLAIER, quad.y0 MULTIPLAIER, vert.position.z },
			vert.color,
			{ quad.s0, quad.t0 },
			this->texture.index
			});
	}
}