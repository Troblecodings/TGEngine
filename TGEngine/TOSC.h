/*
 * TOSC.h
 *
 *  Created on: 17.06.2017
 *      Author: MrTroble
 */
#pragma once
#include "Annotations.h"

enum OS_TYPE {
	WINDOWS,
	ANDROID,
	LINUX,
	MAC,
	XBOX,
	PS,
	SWITCH,
	IOS,
	NaN
};

/*
 * Sets [OS_TYPE* type] to the current OS
 */
SINCE(0, 0, 6)
void getOSType(OS_TYPE type);
