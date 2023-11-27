/** 
    @file driver.c
    @author Ian M Brain (imbrain)
    This file provides the main functionality of the program.
    Users can enter commands to set, get, and remove pairs from the map.
    Usrs can also use commands to print the size of the map as well as quit the program.
    Utilizes functionality from the rest of the program to read in user input,
    parse integer and string values from user input, and to alter map pairs in the map.
*/

#include "value.h"
#include "input.h"
#include "map.h"
#include <string.h>

// Make sure this is OOP style like lecture 21 so hidden implmentation__----___--___---__---__--_--_--_-_-__-__-_-_------__-_-_---

/** Maximum length of user commands including an additional character for invalid commands. */
#define CMD_LENGTH 7

/** Length of the the hash map table within the map component. */
#define TABLE_LENGTH 5

/**
 * Main functionality of the program.
 * Reads user input to set, get, or remove pairs from the map.
 * Also provides functionality to return the size of the map or quit the program.
 * Prints Invlaid command if the user input is invalid.
 * Utilizes functionality from the rest of the program to read in user input,
 * parse integer and string values from user input, and to alter map pairs in the map.
 * The code for this function is based on the main function that I completed in project 4 on 11/1/2023.
 * @param argc number of command line arguments.
 * @param argv array of the command line arguments.
 * @return program exit status.
*/
int main( int argc, char *argv[] )
{
    // User input containing functionality for the program.
    char *user_input = NULL;
    // Command portion of the user input.
    char command[ CMD_LENGTH + 1 ] = "";

    // Map used to provide functionality in the program.
    Map *map = makeMap( TABLE_LENGTH );

    // Continue reading user input until a quit command is inputted or end of file is reached.
    while ( true ) {
        // How many characters have been parsed from user input. Used to read the command and then the key and value.
        int n = 0;
        // Temporary variable to track how many characters are read each time a command or value is parsed.
        int add_n = 0;

        // Parse the current line to the user input.
        user_input = NULL;
        user_input = readLine( stdin );

        // Print cmd> on each line.
        printf( "%s", "cmd> " );

        // If the user does not enter a command, exit the program.
        if ( user_input == NULL ) {
            free( user_input );
            freeMap( map );

            return EXIT_SUCCESS;
        }

        // Parse the command from user input.
        sscanf( user_input, "%s%n", command, &n );

        // Read the key and value information and set them in the map if the command is set.
        if ( strcmp( command, "set" ) == 0 ) {
            // Print the user input.
            printf( "%s\n", user_input );

            // Declare the key and value structs.
            Value key = {};
            Value val = {};

            // Check of whether the key and value are valid.
            bool valid_input = true;

            // Try to parse the key as an integer. If unable to, parse the key as a string.
            add_n = parseInteger( &key, user_input + n );
            if ( add_n == 0 ) {
                // Parse the key as a string.
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
                // Parse the value as a string.
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
            char test_string[ CMD_LENGTH + 1 ] = "";
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
        // Read the key information and set print the corresponding value if the command is get.
        else if ( strcmp( command, "get" ) == 0 ) {
            // Print the user input.
            printf( "%s\n", user_input );

            // Declare the key struct.
            Value key = {};

            // Attempt to parse the key as an integer. If it is not an integer, parse it as a string.
            add_n = parseInteger( &key, user_input + n );
            if ( add_n == 0 ) {
                // Parse the key as a string.
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
            char test_string[ CMD_LENGTH + 1 ] = "";
            sscanf( user_input + n, "%s%n", test_string, &add_n );
            if ( add_n != 0 ) {
                printf( "%s", "Invalid Command\n\n" );
                continue;
            }

            // Get the corresponding value for the map and empty the key that was used.
            Value *val = mapGet( map, &key );
            key.empty( &key );

            // Print undefined if the key does not exist in the map.
            if ( val == NULL )
                printf( "Undefined\n\n" );
            // Print the corresponding vlaue of the key it it exists in the map.
            else {
                val->print( val );
                printf( "\n\n" );
            }
        }
        // Remove the map pair from the map if the the command is remove.
        else if ( strcmp( command, "remove" ) == 0 ) {
            // Print the user input.
            printf( "%s\n", user_input );

            // Declare the key struct.
            Value key = {};
            // Boolean indicating if the map pair was removed.
            bool removed = false;

            // Try to parse the key as an integer. If it is not an integer parse it as a string
            add_n = parseInteger( &key, user_input + n );
            if ( add_n == 0 ) {
                // Parse the key as a string.
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
            char test_string[ CMD_LENGTH + 1 ] = "";
            sscanf( user_input + n, "%s%n", test_string, &add_n );
            if ( add_n != 0 ) {
                printf( "%s", "Invalid Command\n\n" );
                continue;
            }

            // Set the removed boolean to true or false depending on if the map pair was removed.
            removed = mapRemove( map, &key );
            // Free the memory allocated to parse the string.
            key.empty( &key );

            // Print invalid command if the pair corresponding to the key was not removed.
            if ( !removed )
                printf( "Invalid command\n" );

            printf( "\n" );
        }
        // Print the size of the map if the command is size.
        else if ( strcmp( command, "size" ) == 0 ) {
            // Print the user input.
            printf( "%s\n", user_input );

            // Adjust n to check for invalid command inputs.
            add_n = 0;

            // Print invalid command if there are any more strings after the key.
            char test_string[ CMD_LENGTH + 1 ] = "";
            sscanf( user_input + n, "%s%n", test_string, &add_n );
            if ( add_n != 0 ) {
                printf( "%s", "Invalid Command\n\n" );
                continue;
            }

            // Print the size of the map.
            int size_val = mapSize( map );
            printf( "%d\n\n", size_val );
        }
        // Quit the program if the command is quit.
        else if ( strcmp( command, "quit" ) == 0 ) {
            // Adjust n to check for invalid command inputs.
            add_n = 0;

            // Print invalid command if there are any more strings after the key.
            char test_string[ CMD_LENGTH + 1 ] = "";
            sscanf( user_input + n, "%s%n", test_string, &add_n );
            if ( add_n != 0 ) {
                printf( "%s", "Invalid Command\n\n" );
                continue;
            }

            printf( "quit\n" );

            // Free the user input and map memory.
            free( user_input );
            freeMap( map );

            return EXIT_SUCCESS;
        }
        // Print invalid command if the command is not valid or one of the allowed commands.
        else {
            printf( "%s\n", user_input );
            printf( "Invalid Command\n\n" );
        }

        // Free the user input after use.
        free( user_input );
    }

    
}
