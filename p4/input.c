
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Need to return NULL if there are no more lines to read ___------_____---_____----_____----____----____----___--
char *readLine( FILE *fp ) {
    int capacity = 3;
    int len = 1;
    char *text_line = ( char * ) malloc( capacity * sizeof( char ) );
    // does this need to be initialized _______-------____----___-----
    char cur_char = ' ';

    int scan_i = 0;
    scan_i = fscanf( fp, "%c", &cur_char );

    while ( true ) {
        if ( scan_i == EOF ) {
            return NULL;
        }

        if ( cur_char == '\n' ) {
            break;
        }

        if ( len >= capacity ) {
            capacity *= 2;
            text_line = ( char * ) realloc( text_line, capacity * sizeof( char ) );

            //memcpy( new_text_line, text_line, len * sizeof( char ) );

            //free( text_line );
            //text_line = new_text_line;
        }

        text_line[ len - 1 ] = cur_char;
        text_line[ len ] = '\0';
        len++;

        scan_i = 0;
        scan_i = fscanf( fp, "%c", &cur_char );
    }

    return text_line;
}
