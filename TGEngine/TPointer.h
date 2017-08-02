/*
 * TPointer.h
 *
 *  Created on: 17.06.2017
 *      Author: MrTroble
 */
#pragma once

#include <stdio.h>

/*
 * char* fname:
 *  Path to file
 */
struct TPointer {

	/*
	 *Internal FILE*
	 */
	FILE* file;

	/*
	 * File name
	 */
	char* fname;

};

/*
 * Opens a file with read and write permission
 */
void topen(TPointer* pointer);

/*
 * Reads [size of buffer] char's out of the file
 *
 * Note:
 *  open(); needs to be invoked before.
 *  close(); needs to be invoked after.
 */
void tread(TPointer* pointer,char* buffer);

/*
 * Reads the hole file and puts it in the given buffer.
 *
 * Note:
 *  DON'T invoke open(); or close();
 */
void treadFile(TPointer* pointer,char* buffer);

/*
 * Writes [size of buffer] char's to of the file
 *
 * Note:
 *  open(); needs to be invoked before.
 *  close(); needs to be invoked after.
 */
void twrite(TPointer* pointer,char* buffer);

/*
 * Writes [size of buffer] char's to the end of the file
 *
 * Note:
 *  DON'T invoke open(); or close();
 */
void tappande(TPointer* pointer,char* buffer);

/*
 * Closes the FILE*
 */
void tclose(TPointer* pointer);

/*
 * Returns the Length of the Stream
 */
int tlength(TPointer* pointer);
