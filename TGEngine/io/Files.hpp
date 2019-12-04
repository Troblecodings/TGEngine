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

namespace tge {
	namespace nio {

		using namespace std;

		typedef FILE* File;

		/* This is the current working directory of the programm
		 * <strong>This needs to be queryed with initFileSystem()</strong>
		 */
		extern char current_working_dir[];

		/* This holds the offests and sizes of a resource
		 * in a resource file
		 * <ul>
		 * <li><strong class='atr'>offset</strong> is the offset in the file of the resource</li>
		 * <li><strong class='atr'>size</strong></li> is the size in byte of the resource</ul>
		 * <h4>Valid usage</h4>
		 * <li><strong class='atr'>offset</strong> must be in the range from 0 to the size of the file</li>
		 * <li><strong class='atr'>size</strong></li> must be in the range from 1 to the filesize - offset</ul>
		 */
		struct ResourceDescriptor {
			uint64_t offset;
			uint64_t size;
		};

		/*
		 * Querys the current working directory and initalizes all other systems
		 *
		 * Use "current_working_dir" to get the cwd after querying.
		 * (Is automatically called in startTGEngine())
		 */
		void initFileSystem();

		/*
		 * Opens the file
		 * (In debug mode it checks if the open was successful)
		 */
		File open(char* name, char* mode);

		/*
		 * Opens the file and gets the size of the file
		 * (In debug mode it checks if the open was successful)
		 * (the long* needs to be valid pointer, will be checked in debug mode)
		 */
		File readFileSize(char* name, char* mode, /*OUT*/ long* fileLength);

		/*
		 * Reads all bytes of a file
		 * (In debug mode it checks if the open was successful)
		 */
		uint8_t* readAll(char* name);
	}
}
