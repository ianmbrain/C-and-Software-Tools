/**
 * @file input.h
 * @author Ian M Brain (imbrain)
  */

#include <stdio.h>
#include <stdlib.h>

/**
 * Reads a line of text from the specified file.
 * Return null if the end of file is reached and a pointer to a string containing the line otherwise.
 * @param fp file to read the line from.
 * @return pointer to a string that stores the line of text or null if end of file is reached.
*/
char *readLine( FILE *fp );
