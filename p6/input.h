/**
 * @file input.h
 * @author Ian M Brain (imbrain)
 * This file provides functionality to read in a line of text from a file and return a pointer to that string.
 * This functionality is used to read in park lines from in catalog.c and to read in user commands in parks.c.
  */

#include <stdio.h>
#include <stdlib.h>

/** Magnitude to increase capacity by when resizing an array. */
#define DOUBLE_CAPACITY 2

/**
 * Reads a line of text from the specified file.
 * Return null if the end of file is reached and a pointer to a string containing the line otherwise.
 * The code for this function is based on the readLine function that I completed in project 4 on 11/1/2023.
 * @param fp file to read the line from.
 * @return pointer to a string that stores the line of text or null if end of file is reached.
*/
char *readLine( FILE *fp );