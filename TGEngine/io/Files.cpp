#include "Files.hpp"

namespace nio {

	using namespace std;

	char current_working_dir[FILENAME_MAX];

	void initFileSystem()
	{
		GetCurrentDir(current_working_dir, sizeof(current_working_dir));
	}

	File open(char* name, char* mode) {
		File file;
		errno_t err = fopen_s(&file, name, mode);
		ASSERT_NONE_NULL_DB(!err, "Can not open file [" << name << "]!", TG_ERR_FILE_NOT_FOUND_OR_NO_RIGHTS)

		return file;
	}

	File readFileSize(char* name, char* mode, OUTPUT long* file_length) {
		ASSERT_NONE_NULL_DB(file_length, "File length pointer is null!", TG_ERR_DB_NULLPTR)
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