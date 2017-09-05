#pragma once

#include "stdbase.h"
#include <fstream>
#include <vector>
#include <string>

namespace nio {

	using namespace std;

	/*
	 *  Parameter:
	 *      char* fila_path -> path to your file 
	 *  Returns:
	 *      char* -> binary of your file declareted be #file_path
	 */
	SINCE(0, 0, 6)
	vector<char> getBinarys(char* file_path);

}