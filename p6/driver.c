

#include "value.h"
#include "input.h"
#include "map.h"
#include <string.h>

//The driver component is the top-level, main component. 
//Using the other components, it reads and processes commands from standard input, 
// updates the map as needed and prints responses to user commands.

// Make sure this is OOP style like lecture 21 so hidden implmentation__----___--___---__---__--_--_--_-_-__-__-_-_------__-_-_---

int main( int argc, char *argv[] )
{
    char *user_input = NULL;
    char command[ 10 + 1 ] = "";

    Map *map = makeMap( 5 );

    while ( true ) {
        int n = 0;
        int add_n = 0;

        user_input = NULL;
        user_input = readLine( stdin );

        printf( "%s", "cmd> " );

        // If the user does not enter a command, exit the program.
        if ( user_input == NULL ) {
            return EXIT_SUCCESS;
        }

        sscanf( user_input, "%s%n", command, &n );

        if ( strcmp( command, "set" ) == 0 ) {
            printf( "%s\n", user_input );

            // Initialize the key and value structs.
            Value key = {};
            Value val = {};

            // If the key is a string.
            add_n = parseInteger( &key, user_input + n );
            if ( add_n == 0 ) {
                // Parse the key as a string
                add_n = parseString( &key, user_input + n );
            }

            // Command is invalid if a key is not specified.
            if ( add_n == 0 ) {
                printf( "%s", "Invalid Command\n" );
            }

            // Adjust n to read the value.
            n += add_n;

            // If the value is a string
            add_n = parseInteger( &val, user_input + n );
            if ( add_n == 0 ) {
                // Parse the key as a string
                add_n = parseString( &val, user_input + n );
            }

            // Command is invalid if a value is not specified.
            if ( add_n == 0 ) {
                printf( "%s", "Invalid Command\n" );
            }

            // Set the key and value pair in the map.
            mapSet( map, &key, &val );
            printf( "\n" );
        }
        else if ( strcmp( command, "get" ) == 0 ) {
            printf( "%s\n", user_input );

            // Initialize the key and value structs.
            Value key = {};

            add_n = parseInteger( &key, user_input + n );

            // If the key is a string.
            if ( add_n == 0 ) {
                // Parse the key as a string
                add_n = parseString( &key, user_input + n );
            }

            // Command is invalid if a key is not specified.
            if ( add_n == 0 ) {
                printf( "%s", "Invalid Command\n" );
            }

            Value *val = mapGet( map, &key );

            // if ( is_string ) {
            //     char *str_val = val->vptr;
            //     printf( "%s\n", str_val );
            // }
            // else {
            //     printf( "%d\n", val->ival);
            // }

            if ( val == NULL )
                printf( "Undefined\n\n" );
            else {
                val->print( val );
                printf( "\n\n" );
            }
        }
        else if ( strcmp( command, "remove" ) == 0 ) {
            Value key = {};
            bool removed = false;

            add_n = parseInteger( &key, user_input + n );

            // If the key is a string.
            if ( add_n == 0 ) {
                // Parse the key as a string
                add_n = parseString( &key, user_input + n );
            }

            // Command is invalid command if a key is not specified.
            if ( add_n == 0 ) {
                printf( "%s", "Invalid Command\n" );
            }

            removed = mapRemove( map, &key );

            // Print invalid command if the pair corresponding to the key was not removed.
            if ( !removed )
                printf( "Invalid command\n" );
        }
        else if ( strcmp( command, "size" ) == 0 ) {
            printf( "%s\n", command );
            Value key = {};

            // Test if there are any integer values after the command.
            add_n = parseInteger( &key, user_input + n );

            // Test if there are any string values after the command.
            if ( add_n == 0 ) {
                add_n = parseString( &key, user_input + n );
            }

            // Command is invalid command if any other parameters are included
            if ( add_n != 0 )
                printf( "%s", "Invalid Command\n" );

            // Print the size of the map.
            int size_val = mapSize( map );
            printf( "%d\n\n", size_val );
        }
        else if ( strcmp( command, "quit" ) == 0 ) {
            printf( "quit\n" );
            return EXIT_SUCCESS;
        }
        // Print invalid command if the command is not valid
        else {
            printf( "%s\n", user_input );
            printf( "Invalid Command\n" );
        }
    }

    
}
