/*
 * TJson.c
 *
 *  Created on: 18.06.2017
 *      Author: MrTroble
 */

#include "TJson.h"

void json_init(TJson* json){
	topen(json->pointer);
	int tlg = ftell(json->pointer->file);
	if(tlg < 2){
		fseek(json->pointer->file,0,0);
		twrite(json->pointer,"{}");
	}
	tclose(json->pointer);
}

void json_add(TJson* json,char* name, char* value){
	json->pointer->file = fopen(json->name,"a+");
	int tlg = ftell(json->pointer->file);
	fseek(json->pointer->file,tlg-1,0);
	twrite(json->pointer,"\"");
	twrite(json->pointer,name);
	twrite(json->pointer,"\" : \"");
	twrite(json->pointer, value);
	twrite(json->pointer,"\"");
}