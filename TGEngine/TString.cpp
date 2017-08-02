/*
 * TString.c
 *
 *  Created on: 19.06.2017
 *      Author: MrTroble
 */

/**
 * Returns the size of a
 */
#include "TString.h"

void strappend(char* toappend, char* destination, char* buffer) {
	buffer[(SIZE(destination)) + (SIZE(toappend))];
    strcpy_s(buffer,sizeof(destination),destination);
    strcat_s(buffer,sizeof(toappend),toappend);
}
