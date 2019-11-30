#pragma once

#include "..\Stdbase.hpp"

#define printExtend(x) cout << "width:" << x.width << " height:" << x.height << endl; 

#define printOffset(v) cout << "x:" << v.x << " y:" << v.y << endl; 

/*
 * Prints a version with the Format MAJOR.MINOR.VERSION e.g. 1.0.0
 * You can built it with VK_MAKE_VERSION
 */
void printVersion(int version);