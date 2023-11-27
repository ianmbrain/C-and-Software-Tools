/**
 * @file input.c
 * @author Ian M Brain (imbrain)
 * This file provides functionality to read in a line of text from a file and return a pointer to that string.
 * This functionality is used to read in park lines from in catalog.c and to read in user commands in parks.c.
  */

#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/** Initial capacity of the line read in. */
#define INITIAL_LINE_CAPACITY 3

/** Initial length of the line read in. Starts at one to ensure that there is space to set the null termination character. */
#define INITIAL_LINE_LENGTH 1

char *readLine( FILE *fp )
{
    // Capacity of the char array
    int capacity = INITIAL_LINE_CAPACITY;
    // Length of the string. Starts at one to ensure that there is space to set the null termination character.
    int len = INITIAL_LINE_LENGTH;
    // Char pointer that holds the string.
    char *text_line = ( char * ) malloc( capacity * sizeof( char ) );
    // Current character read in from the line.
    char cur_char = ' ';

    // Set to one if the character is read in from the file.
    int scan_i = 0;
    scan_i = fscanf( fp, "%c", &cur_char );

    while ( true ) {
        // Free the text line and return null if end of file is reached.
        if ( scan_i == EOF ) {
            free( text_line );
            return NULL;
        }

        // Stop reading the line when the end of the line is reached.
        if ( cur_char == '\n' ) {
            break;
        }

        // Resize the array when the length reaches capacity.
        if ( len >= capacity ) {
            capacity *= DOUBLE_CAPACITY;
            text_line = ( char * ) realloc( text_line, capacity * sizeof( char ) );
        }

        // Add the current character to the string and increase the length.
        text_line[ len - 1 ] = cur_char;
        text_line[ len ] = '\0';
        len++;

        scan_i = 0;
        scan_i = fscanf( fp, "%c", &cur_char );
    }

    return text_line;
}
