/**
 * @file parks.c
 * @author Ian M Brain (imbrain)
 * Top level file of the parks program.
 * Allows parks to be read in and user commands are read in and performed on those parks.
 * Prints the parks specified by the user commands to output which are redirected to an output file using the command line.
 * Contains functions to compare parks based on their name and id. These are used in the sortParks() function.
 * Contains functions to compare the parks counties to the user inputted counties to determine whcih parks should be printed in listParks().
  */

#include "catalog.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

/** Maximum lngth of any command used to read sections of the command user input. */
#define CMD_LENGTH 6

/** Intial size of the trip to resize the trip array of park pointers. */
#define INITIAL_TRIP_SIZE 5

/**
 * Compares two pointers and determines if va should come before or after vb.
 * Used in the sortParks() function to sort pointers to park pointers in the catalog.
 * Returns -1 if the id of va comes before vb in sorted order, 1 if va comaes after vb, and 0 otherwise.
 * @param va pointer a pointer a park in the catalog list.
 * @param vb pointer a pointer a park in the catalog list.
 * @return -1 if va comes before vb in sorted order, 1 if va comaes after vb, and 0 otherwise.
*/
static int idComp( void const *va, void const *vb )
{
    Park * const *p1 = va;
    Park * const *p2 = vb;

    if ( (*( *p1 )).id < (*( *p2 )).id ) {
        return -1;
    }
    else if ( (*( *p1 )).id > (*( *p2 )).id ) {
        return 1;
    }
    else {
        return 0;
    }   
}

/**
 * Compares two pointers and determines if va should come before or after vb.
 * Used in the sortParks() function to sort pointers to park pointers in the catalog.
 * Returns -1 if the name of va comes before vb in sorted order and 1 if va comaes after vb.
 * Use idComp to sort by the park id if the names of the parks match.
 * @param va pointer a pointer a park in the catalog list.
 * @param vb pointer a pointer a park in the catalog list.
 * @return -1 if va comes before vb in sorted order, 1 if va comaes after vb, and 0 otherwise.
*/
static int nameComp( void const *va, void const *vb )
{
    Park * const *p1 = va;
    Park * const *p2 = vb;

    int str_cmp = strcmp( (*( *p1 )).name, (*( *p2 )).name );
    if ( str_cmp == 0 ) {
        return idComp( va, vb );
    }
    else if ( str_cmp < 0 ) {
        return -1;
    }
    else {
        return 1;
    }
}

/**
 * Compares the specified parks counties with the parameter county.
 * Include the park in output if the parameter county matches a county of the park.
 * Otherwise do not include the park in output.
 * Provides functionality in listParks() to specify which parks should be included in output based on the user inputted county.
 * @param park park to test if it should be included in output.
 * @param str string to compare the park against.
 * @return true if the park should be included in output and false otherwise.
*/
static bool testCounty( Park const *park, char const *str ) {
    // Number of counties associated with the park.
    int num_counties = 0;
    num_counties = sizeof( park->counties ) / sizeof( park-> counties[ 0 ] );

    for ( int i = 0; i < num_counties; i++ ) {
        if ( strcmp( park->counties[ i ], str ) == 0 ) {
            return true;
        }
    }

    // Return false if the park is not associated with the county.
    return false;
}
/**
 * This function is used when the user command is list parks or list names to lits every park in the catalog.
 * Provides functionality in listParks() to specify that all parks in the catalog should be printed.
 * @param park park to test if it should be included in output.
 * @param str string to compare the park against.
 * @return true if the park should be included in output and false otherwise.
*/
static bool testTrue( Park const *park, char const *str ) {
    return true;
}

/**
 * Struct representing the trip of parks.
 * Holds a pointer to an array of pointers to parks.
 * Contains the count of parks in the trip and a capacity variable to resize the array of park pointers.
*/
typedef struct TripStruct {
    Park **list;
    int count;
    int capacity;
} Trip;

/**
 * Main functionality of the program.
 * Allocates a trip and catalog and reads in the parks from a park file to the catalog.
 * Reads in user commands list the parks by park id, county, or name.
 * Reads in user commands to add and remove parks to trips and print out the parks in the trip.
 * Frees memory of the catalog, trip, parks, and user input after running.
 * @param argc number of command line arguments.
 * @param argv array of the command line arguments.
 * @return program exit status.
*/
int main( int argc, char *argv[] ) {
    // Create and allocate a new catalog.
    Catalog *catalog = makeCatalog();

    // Throw and error if the input does not contain any files to read.
    if ( argc == 1 ) {
        freeCatalog( catalog );

        fprintf( stderr, "%s\n", "usage: parks <park-file>*" );
        exit( EXIT_FAILURE );
    }

    // Read parks from inputted files into the catalog. Starts at one as the first argument is the executable.
    for ( int i = 1; i < argc; i++ ) {
        readParks( argv[ i ], catalog );
    }

    // Allocate memory for the trip and the array of park pointers stored in the list.
    Trip *trip = ( Trip * ) malloc( sizeof( Trip ) );
    trip->list = (Park **) malloc( INITIAL_TRIP_SIZE * sizeof( Park * ) );
    trip->count = 0;
    trip->capacity = INITIAL_TRIP_SIZE;

    // Input of the user read fro mthe input file.
    char *user_input = NULL;
    // Command to be parsed from the user input.
    char command[ CMD_LENGTH + 1 ] = "";

    while( true ) {
        // Variable to track how many characters have been read from the string user input.
        int n = 0;
        // Variable to help track the read characters.
        int add_n = 0;

        // Read in the user input from the input file.
        user_input = readLine( stdin );

        printf( "%s", "cmd> " );

        // Free memory and exit the program if the user input is quit or not included.
        if ( user_input == NULL || strcmp( user_input, "quit" ) == 0 ) {
            if ( user_input != NULL )
                printf( "%s\n", user_input );

            free( user_input );
            freeCatalog( catalog );
            free( trip->list );
            free( trip );

            exit( EXIT_SUCCESS );
        }

        // Read the command of the user input.
        sscanf( user_input, "%s%n", command, &n );

        // Read the next part of the command if the user command is list.
        if ( strcmp( command, "list" ) == 0 ) {
            sscanf( user_input + n, "%s%n", command, &add_n );
            n += add_n;

            // Sort the parks by their id and print them to output if the command is parks.
            if ( strcmp( command, "parks") == 0 ) {
                printf( "%s\n", user_input );

                sortParks( catalog, idComp );
                listParks( catalog, testTrue, NULL );
            }

            // Read in the county and print out parks with those counties sorted by name and id if the command is county.
            else if ( strcmp( command, "county") == 0 ) {
                printf( "%s\n", user_input );

                // County specified in the user input.
                char county[ COUNTY_NAME_LENGTH + 1 ] = "";
                
                // Print Invalid command if the user does not specify a county.
                if ( sscanf( user_input + n, "%s", county ) != 1 ) {
                    fprintf( stderr, "%s\n", "Invalid command" );
                    free( user_input );
                    continue;
                }

                // Print Invalid command if the length of the county name is too long.
                else if ( county[ COUNTY_NAME_LENGTH ] != '\0' ) {
                    fprintf( stderr, "%s\n", "Invalid command" );
                    free( user_input );
                    continue;
                }

                sortParks( catalog, nameComp );
                listParks( catalog, testCounty, county );
            }

            // Sort the parks by their name and id and print them to output if the command is names.
            else if ( strcmp( command, "names") == 0 ) {
                printf( "%s\n", user_input );
                sortParks( catalog, nameComp );
                listParks( catalog, testTrue, NULL );
            }
        }

        // Add the specified park to the trip if the command is add.
        else if ( strcmp( command, "add" ) == 0 ) {
            // Park id specified next in the command.
            int park_id = 0;
            sscanf( user_input + n, "%d%n", &park_id, &n );

            // Search for the park to add from the catalog.
            Park *add_park = NULL;

            // Print Invalid command if the id is not an int.
            if ( park_id == -1 ) {
                fprintf( stderr, "%s\n", "Invalid command" );
                free( user_input );
                continue;
            }

            // Set the added park pointer to the corresponding park in the catalog. Park remains null if it does not have a matching id.
            for ( int i = 0; i < catalog->count; i++ ) {
                if ( catalog->list[ i ]->id == park_id ) {
                    add_park = catalog->list[ i ];
                }
            }

            // Print Invalid command if the id does not match an existing park id.
            if ( add_park == NULL ) {
                printf( "%s\n", user_input );
                free( user_input );
                printf( "%s\n\n", "Invalid command" );
                continue;
            }

            // Add the park to the trip and resize the trip array when capacity is reached.
            if ( trip->count >= trip->capacity ) {
                trip->capacity *= 2;
                trip->list = (Park **) realloc( trip->list, trip->capacity * sizeof( Park * ) );
            }

            trip->list[ trip->count ] = add_park;
            trip->count++;

            printf( "%s\n", user_input );
        }

        // Remove the specified park from the trip if the command is remove.
        else if ( strcmp( command, "remove" ) == 0 ) {
            // Id of the park to remove.
            int park_id = -1;
            sscanf( user_input + n, "%d%n", &park_id, &n );
            // Boolean specifying if the park id matches a park in the trip
            bool remove_park = false;

            // Print Invalid command if the id is not an integer
            if ( park_id == -1 ) {
                fprintf( stderr, "%s\n", "Invalid command" );
                free( user_input );
                continue;
            }

            // Search for the park to remove from the trip.
            for ( int i = 0; i < trip->count; i++ ) {
                if ( trip->list[ i ]->id == park_id ) {
                    remove_park = true;
                    for ( int j = i; j < trip -> count - 1; j++ ) {
                        trip->list[ j ] = trip->list[ j + 1 ];
                    }

                    trip->count--;
                    break;
                }
            }

            // Print Invalid command error if the id does not match an existing park id in the trip.
            if ( remove_park == false ) {
                printf( "%s\n", user_input );
                printf( "%s\n\n", "Invalid command" );
                free( user_input );
                continue;
            }

            printf( "%s\n", user_input );
        }

        // Print out the parks in the trip specifying their id, name, and cumulative distance of the trip.
        else if ( strcmp( command, "trip" ) == 0 ) {
            // Total distance of the trip updated with each park
            double total_distance = 0;
            printf( "%s\n", user_input );

            printf( "%s\n", "ID  Name                                     Distance" );

            for ( int i = 0; i < trip->count; i++ ) {
                printf( "%-3d ", trip->list[ i ]->id );
                printf( "%-40s ", trip->list[ i ]->name );

                if ( i > 0 ) {
                    total_distance += distance( trip->list[ i ], trip->list[ i - 1 ] );
                }
                printf( "%8.1f\n", total_distance );
            }
        }
        
        // Print Invalid command if the user enters and invalid command.
        else {
            printf( "%s\n", user_input );
            printf( "%s\n", "Invalid command" );
        }

        free( user_input );
        user_input = NULL;
        printf( "%s", "\n" );
    }
}
