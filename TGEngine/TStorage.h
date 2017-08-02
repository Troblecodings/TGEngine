/*
 * IStorage.h
 *
 *  Created on: 17.06.2017
 *      Author: MrTroble
 */
#pragma once

#include "TPointer.h"
#include "TString.h"
#include "TOSC.h"

/*
 * char* bp:
 *  Is the base path.
 */
struct IStorage{

	/*
	 * File name
	 */
	char* bp;

} ;

/*
 * Creates a new file with [name] in base path folder
 *
 * Note:
 *  [name] can contain sub-folders
 */
int addFile(IStorage* storage,char* name);

/*
 * Creates a new folder with [name] in base path folder
 *
 * Note:
 *  [name] can contain sub-folders
 
int addFolder(IStorage* storage,char* name);
 */

/*
 * Returns a TPointer to the file
 */
void getPointer(IStorage* storage,char* name,TPointer* pointer);

