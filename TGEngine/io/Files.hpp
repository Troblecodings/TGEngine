#pragma once

#include "..\Stdbase.hpp"
#include <fstream>
#include <string>

#ifdef WIN32
#include <direct.h>
#define GetCurrentDir getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

namespace nio {

	using namespace std;

	extern char current_working_dir[];

	/*
	 * Querys the current working directory 
	 * 
	 * Use "current_working_dir" to get the cwd after quereing.
	 * (Is automatically called in initTGEngine())
	 */
	SINCE(0, 0, 3)
	void queryCWD();

	/*
	 *  Parameter:
	 *      char* fila_path -> path to your file 
	 *  Returns:
	 *      char* -> binary of your file declareted be #file_path
	 */
	SINCE(0, 0, 1)
	vector<char> getBinarys(char* file_path);

	/*
	 * If size_of_file is not nullptr it returns the filesize 
	 */
	SINCE(0, 0, 2)
	ifstream getBinaryStream(INPUT char* filename, OUTPUT size_t* size_of_file);
}