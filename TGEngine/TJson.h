/*
 * TJson.h
 *
 *  Created on: 20.06.2017
 *      Author: MrTroble
 */
#pragma once

#include "TPointer.h"

struct TJson{
	TPointer* pointer;
	char* name;
};

void json_init(TJson* json);

void json_add(TJson* json,char* name, void* value);