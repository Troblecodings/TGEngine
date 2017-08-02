/*
 * TPointer.h
 *
 *  Created on: 17.06.2017
 *      Author: MrTroble
 */

#include "TPointer.h"

void topen(TPointer* pointer){
	pointer->file = fopen(pointer->fname,"r+");
}

void tread(TPointer* pointer,char* buffer){
	fread(buffer,sizeof(char),sizeof(buffer),pointer->file);
}

void treadFile(TPointer* pointer, char* buffer){
	FILE* fs = fopen(pointer->fname,"a+");
	buffer[ftell(fs)];
	fseek(fs,0,0);
	fread(buffer,sizeof(char),sizeof(buffer),fs);
	fclose(fs);
}

void twrite(TPointer* pointer,char* buffer){
	fwrite(buffer,sizeof(char),sizeof(buffer),pointer->file);
}

void tappande(TPointer* pointer,char* buffer){
	FILE* ptr = fopen(pointer->fname,"a");
	fwrite(buffer,sizeof(char),sizeof(buffer),ptr);
	fclose(ptr);
}

void tclose(TPointer* pointer){
	fclose(pointer->file);
}

int tlength(TPointer* pointer){
	FILE* fs = fopen(pointer->fname,"a");
	int rtv = ftell(fs);
	fclose(fs);
	return rtv;
}
