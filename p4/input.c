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

/**
 * Reads a line of text from the specified file.
 * Return null if the end of file is reached and a pointer to a string containing the line otherwise.
 * @param fp file to read the line from.
 * @return pointer to a string that stores the line of text or null if end of file is reached.
*/
char *readLine( FILE *fp ) {
    // Capacity of the char array
    int capacity = 3;
    // Length of the string. Starts at one to ensure that there is space to set the null termination character.
    int len = 1;
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
            capacity *= 2;
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
