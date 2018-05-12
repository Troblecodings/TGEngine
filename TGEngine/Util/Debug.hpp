#pragma once

#include "..\stdbase.hpp"
#include <time.h>
#include <chrono>
#include <thread>
#include <fbxsdk.h>

SINCE(0, 0, 1)
#define printExtend(x) cout << "width:" << x.width << " height:" << x.height << endl; 

SINCE(0, 0, 1)
#define printOffset(v) cout << "x:" << v.x << " y:" << v.y << endl; 

/*
 * Prints a version with the Format MAJOR.MINOR.VERSION e.g. 1.0.0
 * You can built it with VK_MAKE_VERSION
 */
SINCE(0, 0, 1)
void printVersion(int version);

/*
 * Throws a critical error 
 */
SINCE(0, 0, 1)
void error(char* errorname, int errorcode);

SINCE(0, 0, 1)
void ErrorCallback(int, const char* err_str);

SINCE(0, 0, 3)
void PrintNode(FbxNode* pNode);

SINCE(0, 0, 3)
void PrintAttribute(FbxNodeAttribute* pAttribute);

SINCE(0, 0, 3)
FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);