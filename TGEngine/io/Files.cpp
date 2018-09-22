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

	File open(char* name, char* mode) {
		File file = fopen(name, mode);
#ifdef DEBUG
		if (!file) {
			cerr << "Error can't open file [" << name << "]" << endl;
			return nullptr;
		}
#endif
		return file;
	}

	File readFileSize(char* name, char* mode, OUTPUT long* file_length) {
#ifdef DEBUG
		if (!file_length) {
			cerr << "On call readFileSize the paramter file_length was nullptr [should be a valid long pointer]" << endl;
			return nullptr;
		}
#endif
		File file = open(name, mode);

		fseek(file, 0, SEEK_END);
		*file_length = ftell(file);
		fseek(file, 0, SEEK_SET);
		return file;
	}

	uint8_t* readAll(char* name) {
		long size = 0;
		File file = readFileSize(name, "rb", &size);
		uint8_t* data = new uint8_t[size];
		fread(data, sizeof(uint8_t), size, file);
		return data;
	}
}