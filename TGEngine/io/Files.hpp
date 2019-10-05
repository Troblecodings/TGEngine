#pragma once

#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <iostream>
#include "../util/Annotations.hpp"
#include "../Error.hpp"

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#define TGE_GET_RESOURCE(EXEC) for (uint32_t i = 0; i < size; i++) \
{\
	ResourceDescriptor* tex = &input[i];\
    if (ftell(file) != tex->offset)\
    fseek(file, tex->offset, SEEK_SET); \
    stbi_uc* imgbuffer = new stbi_uc[tex->size]; \
    fread(imgbuffer, sizeof(char), tex->size, file); \
    EXEC\
}

namespace tge {
	namespace nio {

		using namespace std;

		typedef FILE* File;

		extern char current_working_dir[];

		/*
		 * Querys the current working directory and initalizes all other systems
		 *
		 * Use "current_working_dir" to get the cwd after quereing.
		 * (Is automatically called in startTGEngine())
		 */
		SINCE(0, 0, 3)
			void initFileSystem();

		/*
		 * Opens the file
		 * (In debug mode it checks if the open was successfull)
		 */
		SINCE(0, 0, 4)
			File open(char* name, char* mode);

		/*
		 * Opens the file and get's the size of the file
		 * (In debug mode it checks if the open was successfull)
		 * (the long* needs to be vailid pointer, will be checked in debug mode)
		 */
		SINCE(0, 0, 4)
			File readFileSize(char* name, char* mode, /*OUT*/ long* fileLength);

		/*
		 * Reads all bytes out of a file
		 * (In debug mode it checks if the open was successfull)
		 */
		SINCE(0, 0, 4)
			uint8_t* readAll(char* name);
	}
}