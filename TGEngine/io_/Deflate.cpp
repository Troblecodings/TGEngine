#include "Deflate.hpp"

std::vector<uint8_t> inflate_s(std::vector<uint8_t> data) {
	std::vector<bool> dta;
	for(uint8_t dt : data){
		std::cout << static_cast<int>(dt) << std::endl;
		if (dt == 256) {
			std::cout << "EndofCode" << std::endl;
			break;
		}
		uint32_t pos = dta.size();
		dta.resize(pos + 8);
		std::cout << (dta[pos + 0] = ((dt & 128) == 128));
		std::cout << (dta[pos + 1] = ((dt & 64) == 64));
		std::cout << (dta[pos + 2] = ((dt & 32) == 32));
		std::cout << (dta[pos + 3] = ((dt & 16) == 16));
		std::cout << (dta[pos + 4] = ((dt & 8) == 8));
		std::cout << (dta[pos + 5] = ((dt & 4) == 4));
		std::cout << (dta[pos + 6] = ((dt & 2) == 2));
		std::cout << (dta[pos + 7] = ((dt & 1))) << std::endl;
	}
	for (bool binary : dta) {
		if (binary) {

		}
	}
	return data;
}