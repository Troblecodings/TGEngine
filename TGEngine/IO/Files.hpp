#pragma once

#include "..\stdbase.hpp"
#include <fstream>
#include <string>

namespace nio {

	using namespace std;

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