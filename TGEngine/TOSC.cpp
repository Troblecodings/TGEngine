#include "TOSC.h"

void getOSType(OS_TYPE type) {
    #ifdef _WIN32 || _WIN64
	    type = WINDOWS;
    #elif __ANDROID__
	    type = ANDROID;
    #elif __linux__
	    type = LINUX;
    #elif __APPLE__ || __MACH__	|| macintosh
	    type = MAC;
    #endif
}