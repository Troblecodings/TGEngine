/*
 * TStorage.c
 *
 *  Created on: 17.06.2017
 *      Author: MrTroble
 */
#include "TStorage.h"

/*
 * Returns the Storage struct for OS
 */
IStorage* getStorage(char* path,char* device){
	OS_TYPE type;
	getOSType(type);
	if(type == WINDOWS){
		char cpopath[SIZE(path)];
		strcpy_s(cpopath, sizeof(path),path);
		strappend(cpopath,device, path);
	}
	IStorage* storage;
	storage->bp = path;
	return storage;
}

int addFile(IStorage* storage,char* name){
	char* buffer;
	strappend(name, storage->bp,buffer);
	return fclose(fopen(buffer,"w")) == 0;
}

/*int addFolder(IStorage* storage,char* name){
	char* buffer;
	strappend(name, storage->bp, buffer);
	return mkdir(buffer, 0400 | 0200);
}*/

void getPointer(IStorage* storage,char* name, TPointer* pointer){
	strappend(storage->bp,name,pointer->fname);
}
