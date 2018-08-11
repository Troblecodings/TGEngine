#include "Files.hpp"

namespace nio {

	using namespace std;

	char current_working_dir[FILENAME_MAX];

	void queryCWD()
	{
		GetCurrentDir(current_working_dir, sizeof(current_working_dir));
	}

	vector<char> getBinarys(char* filepath) {
		size_t size_of_file;
		ifstream inputstream = getBinaryStream(filepath, &size_of_file);
		vector<char> buffer(size_of_file);
		inputstream.read(buffer.data(), size_of_file);
		inputstream.close();
		return buffer;
	}

	ifstream getBinaryStream(INPUT char* filename,OUTPUT size_t* size_of_file) {
		ifstream strm(filename, ios::binary | ios::ate);
		if(size_of_file != nullptr)*size_of_file = strm.tellg();
		strm.seekg(0);
		return strm;
	}
}