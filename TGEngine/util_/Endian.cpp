#include "Endian.hpp"

unsigned int int_from_big_endian(unsigned char* data, int start_index) {
	return (data[start_index] << 24)
		| (data[start_index + 1] << 16)
		| (data[start_index + 2] << 8)
		| data[start_index + 3];
}