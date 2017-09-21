#include "Files.hpp"

namespace nio {

	using namespace std;

	vector<char> getBinarys(char* filepath) {
		ifstream inputstream(filepath, ios::binary | ios::ate);
		size_t size_of_file = inputstream.tellg();
		vector<char> buffer(size_of_file);
		inputstream.seekg(0);
		inputstream.read(buffer.data(), size_of_file);
		inputstream.close();
		return buffer;
	}
}