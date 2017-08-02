/*
 * TOSC.h
 *
 *  Created on: 17.06.2017
 *      Author: MrTroble
 */
#pragma once

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
void getOSType(OS_TYPE type);
