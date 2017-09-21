#pragma once

#include "TGEngine.hpp"
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
	SINCE(0, 0, 1)
	vector<char> getBinarys(char* file_path);

}