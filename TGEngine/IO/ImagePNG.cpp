#include "ImagePNG.hpp"

void loadPNGData(Texture* texture) {
	FILE* fileptr = fopen(texture->texture_path, "rb");

	char PNG_START[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };

	std::vector<uint8_t> buffer(8);
	std::vector<Color> PLTE;
	fread(buffer.data(), sizeof(unsigned char), 8, fileptr);
	OUT_LV_DEBUG(buffer.data())
	if (memcmp(buffer.data(), PNG_START, 8) == 0) {
		OUT_LV_DEBUG("Valide PNG")
		while (1) {
			fread(buffer.data(), sizeof(unsigned char), 8, fileptr);
			if (BUFFER_COMPARE(buffer, "IHDR")) {
				uint32_t chunksize = int_from_big_endian(buffer.data(), 0); 
				if (chunksize != 13) {
					OUT_LV_DEBUG("Error! Wrong chunksize on IHDR")
					return;
				}
				buffer.resize(chunksize); 
				fread(buffer.data(), sizeof(unsigned char), chunksize, fileptr);
				texture->texture_extend.width = int_from_big_endian(buffer.data(), 0);
				texture->texture_extend.height = int_from_big_endian(buffer.data(), 4);
				texture->depth = buffer[8]; // I'll ignore that for now
				texture->color_type = buffer[9]; // Some how 3 & 6
				texture->compression_methode = buffer[10]; // Ignore should be 0
				texture->filter_methode = buffer[11]; // Ignore should be 0
				texture->interlace_methode = buffer[12]; // Ignore

				if (texture->compression_methode != 0) {
					OUT_LV_DEBUG("Only deflate compression is allowed")
					return;
				}
				if (texture->filter_methode != 0) {
					OUT_LV_DEBUG("Only none filters are allowed")
					return;
				}
				OUT_LV_DEBUG("Interlace is being ignored")

			}
			else if (BUFFER_COMPARE(buffer, "PLTE")) {
				uint32_t chunksize = int_from_big_endian(buffer.data(), 0);
				if (texture->color_type != COLOR_TYPE_INDEXED) {
					OUT_LV_DEBUG("No PLTE chunk needed! Why is this even in here? (Color type is not indexed)")
					fseek(fileptr, (int)ftell(fileptr) + chunksize + 4, 0);
					continue;
				}
				if (chunksize % 3 != 0) {
					OUT_LV_DEBUG("Error! Wrong chunksize on PLTE (Not divisible by 3)")
					return;
				}
				PLTE.resize(chunksize / 3);
				for (size_t i = 0; i < PLTE.size(); i++)
				{
					fread(&PLTE[i].r, sizeof(uint8_t), 1, fileptr);
					fread(&PLTE[i].g, sizeof(uint8_t), 1, fileptr);
					fread(&PLTE[i].b, sizeof(uint8_t), 1, fileptr);
				}
			}
			else if (BUFFER_COMPARE(buffer, "IDAT")) {
				OUT_LV_DEBUG("Start IDAT png data deflate")
				uint32_t chunksize = int_from_big_endian(buffer.data(), 0);
				buffer.resize(chunksize - 6);
				uint8_t flags, check;
				fread(&flags, sizeof(uint8_t), 1, fileptr);
				fread(&check, sizeof(uint8_t), 1, fileptr);

				fread(buffer.data(), sizeof(uint8_t), buffer.size(), fileptr);
				inflate_s(buffer);
				fseek(fileptr, 4, SEEK_CUR);
			}
			else if (BUFFER_COMPARE(buffer, "IEND")) {
				OUT_LV_DEBUG("IEND")
				break;
			}
			else {
				OUT_LV_DEBUG("Skipping chunk as unneccesary")
				fseek(fileptr, (int)int_from_big_endian(buffer.data(), 0), SEEK_CUR);
			}
			fseek(fileptr, 4, SEEK_CUR);
		}
	}
}