/*
 * TString.h
 *
 *  Created on: 17.06.2017
 *      Author: MrTroble
 */
#pragma once

#include <string.h>

#define SIZE(a) sizeof(a)/sizeof(*a)

/**
 * Appends [char* toappend] to [char* destination]
 */
void strappend(char* toappend, char* destination, char* buffer);