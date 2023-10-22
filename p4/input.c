

#include <stdio.h>

char *readLine( FILE *fp ) {
    int capacity = 3;
    int len = 0;
    char *text_line = (char *) malloc( capacity * sizeof( char ) );
    // does this need to be initialized _______-------____----___-----
    char cur_char;

    while ( fscanf( fp, "%c", &cur_char ) == 1 ) {
        if ( cur_char == '\n' ) {
            break;
        }

        if ( len >= capacity ) {
            capacity *= 2;
            text_line = (char *) realloc( text_line, capacity * sizeof( char ) );
        }

        len++;
        text_line[ len ] = cur_char;
    }

    return text_line;
}
