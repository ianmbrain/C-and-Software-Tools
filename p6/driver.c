

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
            free( user_input );
            freeMap( map );

            return EXIT_SUCCESS;
        }

        sscanf( user_input, "%s%n", command, &n );

        if ( strcmp( command, "set" ) == 0 ) {
            printf( "%s\n", user_input );

            // Initialize the key and value structs.
            Value key = {};
            Value val = {};

            // Check of whether the key and value are valid.
            bool valid_input = true;

            // Try to parse the key as an integer. If unable to, parse the key as a string.
            add_n = parseInteger( &key, user_input + n );
            if ( add_n == 0 ) {
                // Parse the key as a string
                add_n = parseString( &key, user_input + n );
            }

            // Command is invalid if a key is not specified.
            if ( add_n == 0 ) {
                printf( "%s", "Invalid Command\n" );
                valid_input = false;
            }

            // Adjust n to read the value.
            n += add_n;
            add_n = 0;

            // Try to parse the value as an integer. If unable to, parse the value as a string.
            add_n = parseInteger( &val, user_input + n );
            if ( add_n == 0 ) {
                // Parse the value as a string
                add_n = parseString( &val, user_input + n );
            }

            // Command is invalid if a value is not specified.
            if ( add_n == 0 ) {
                printf( "%s", "Invalid Command\n" );
                valid_input = false;
            }

            // Adjust n to check for invalid command inputs.
            n += add_n;
            add_n = 0;

            // Print invalid command if there are any more strings in the command aside from the command, key, and value.
            char test_string[ 20 ] = "";
            sscanf( user_input + n, "%s%n", test_string, &add_n );
            if ( add_n != 0 ) {
                printf( "%s", "Invalid Command\n" );
                valid_input = false;
            }

            // Set the key and value pair in the map.
            if ( valid_input )
                mapSet( map, &key, &val );

            printf( "\n" );
        }
        else if ( strcmp( command, "get" ) == 0 ) {
            printf( "%s\n", user_input );

            // Initialize the key and value structs.
            Value key = {};

            // Attempt to parse the key as an integer. If it is not an integer, parse it as a string.
            add_n = parseInteger( &key, user_input + n );
            if ( add_n == 0 ) {
                // Parse the key as a string
                add_n = parseString( &key, user_input + n );
            }

            // Command is invalid if a key is not specified.
            if ( add_n == 0 ) {
                printf( "%s", "Invalid Command\n\n" );
                continue;
            }

            // Adjust n to check for invalid command inputs.
            n += add_n;
            add_n = 0;

            // Print invalid command if there are any more strings after the key.
            char test_string[ 20 ] = "";
            sscanf( user_input + n, "%s%n", test_string, &add_n );
            if ( add_n != 0 ) {
                printf( "%s", "Invalid Command\n\n" );
                continue;
            }

            Value *val = mapGet( map, &key );
            key.empty( &key );

            if ( val == NULL )
                printf( "Undefined\n\n" );
            else {
                val->print( val );
                printf( "\n\n" );
            }
        }
        else if ( strcmp( command, "remove" ) == 0 ) {
            printf( "%s\n", user_input );

            Value key = {};
            bool removed = false;

            // Try to parse the key as an integer. If it is not an integer parse it as a string
            add_n = parseInteger( &key, user_input + n );
            if ( add_n == 0 ) {
                // Parse the key as a string
                add_n = parseString( &key, user_input + n );
            }

            // Command is invalid command if a key is not specified or it is in an invalid format.
            if ( add_n == 0 ) {
                printf( "%s", "Invalid Command\n\n" );
                continue;
            }

            // Adjust n to check for invalid command inputs.
            n += add_n;
            add_n = 0;

            // Print invalid command if there are any more strings after the key.
            char test_string[ 20 ] = "";
            sscanf( user_input + n, "%s%n", test_string, &add_n );
            if ( add_n != 0 ) {
                printf( "%s", "Invalid Command\n\n" );
                continue;
            }

            removed = mapRemove( map, &key );
            // Free the memory allocated to parse the string
            key.empty( &key );

            // Print invalid command if the pair corresponding to the key was not removed.
            if ( !removed )
                printf( "Invalid command\n" );

            printf( "\n" );
        }
        else if ( strcmp( command, "size" ) == 0 ) {
            printf( "%s\n", command );

            // // Test if there are any integer values after the command.
            // add_n = parseInteger( &key, user_input + n );

            // // Test if there are any string values after the command.
            // if ( add_n == 0 ) {
            //     add_n = parseString( &key, user_input + n );

            //     // Free the memory used to parse any remaining string values after the command.
            //     key.empty( &key );
            // }

            // Adjust n to check for invalid command inputs.
            add_n = 0;

            // Print invalid command if there are any more strings after the key.
            char test_string[ 20 ] = "";
            sscanf( user_input + n, "%s%n", test_string, &add_n );
            if ( add_n != 0 ) {
                printf( "%s", "Invalid Command\n\n" );
                continue;
            }

            // Print the size of the map.
            int size_val = mapSize( map );
            printf( "%d\n\n", size_val );
        }
        else if ( strcmp( command, "quit" ) == 0 ) {
            // Adjust n to check for invalid command inputs.
            add_n = 0;

            // Print invalid command if there are any more strings after the key.
            char test_string[ 20 ] = "";
            sscanf( user_input + n, "%s%n", test_string, &add_n );
            if ( add_n != 0 ) {
                printf( "%s", "Invalid Command\n\n" );
                continue;
            }

            printf( "quit\n" );

            free( user_input );
            freeMap( map );

            return EXIT_SUCCESS;
        }
        // Print invalid command if the command is not valid
        else {
            printf( "%s\n", user_input );
            printf( "Invalid Command\n\n" );
        }

        free( user_input );
    }

    
}
