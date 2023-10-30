// Contains Main()
// You can define the comparison functions in your top-level parks component and make them static

#include "catalog.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

#define CMD_LENGTH 6

#define INITIAL_TRIP_SIZE 5


static int idComp( void const *va, void const *vb )
{
    // const Park p1 = *(Park const *) va;
    // const Park p2 = *(Park const *) vb;
    Park * const *p1 = va;
    Park * const *p2 = vb;
    // const Park p1 = **(( Park * const * ) va);
    // const Park p2 = **(( Park * const * ) va);
    //fprintf( stderr, "%p   ", p1 );
    //fprintf( stderr, "%d\n", (*p1)->id );
    // fprintf( stderr, "%d\n", p2.id );

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

static int nameComp( void const *va, void const *vb )
{
    Park * const *p1 = va;
    Park * const *p2 = vb;

    int str_cmp = strcmp( (*( *p1 )).name, (*( *p2 )).name );
    //fprintf( stderr, "%s\n", p1->name);
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

static bool testCounty( Park const *park, char const *str ) {
    int num_counties = sizeof( park->counties ) / sizeof( park-> counties[ 0 ] );
    for ( int i = 0; i < num_counties; i++ ) {
        // Return true if the park is associated with the county.
        if ( strcmp( park->counties[ i ], str ) == 0 ) {
            return true;
        }
    }

    // Return false if the park is not associated with the county.
    return false;
}

static bool testTrue( Park const *park, char const *str ) {
    return true;
}

struct TripStruct {
    Park **list;
    int count;
    int capacity; 
};
typedef struct TripStruct Trip;

int main( int argc, char *argv[] ) {
    //CITE THIS CODE from catalog ___--__---_-_----_-_-_--_----_---_--_-_-_-__--
    Trip *trip = ( Trip * ) malloc( sizeof( Trip ) );
    trip->list = (Park **) malloc( INITIAL_TRIP_SIZE * sizeof( Park * ) );
    trip->count = 0;
    trip->capacity = INITIAL_TRIP_SIZE;

    // Create a new catalog.
    Catalog *catalog = makeCatalog();

    // Throw and error if the input does not contain any files to read.
    if ( argc == 1 ) {
        fprintf( stderr, "%s\n", "usage: parks <park-file>*" );
        exit( EXIT_FAILURE );
    }

    // Read parks from inputted files into the catalog. Starts at one as the first argument is the executable.
    for ( int i = 1; i < argc; i++ ) {
        readParks( argv[ i ], catalog );
    }

    char *user_input = NULL;
    char command[ CMD_LENGTH + 1 ] = "";

    while( true ) {
        int n = 0;
        int add_n = 0;
        user_input = "";
        user_input = readLine( stdin );
        //fprintf( stderr, "%s\n", user_input );

        printf( "%s", "cmd> " );

        if ( user_input == NULL || strcmp( user_input, "quit" ) == 0 ) {
            for ( int i = 0; i < catalog->count; i++ ) {
                free( catalog->list[ i ] );
            }

            if ( user_input != NULL )
                printf( "%s\n", user_input );

            free( user_input );
            free( catalog->list );
            free( catalog );
            free( trip->list );
            free( trip );

            exit( EXIT_SUCCESS );
        }

        // Read the command of the user input.
        sscanf( user_input, "%s%n", command, &n );

        if ( strcmp( command, "list" ) == 0 ) {
            sscanf( user_input + n, "%s%n", command, &add_n );
            n += add_n;
            //fprintf( stderr, "%s\n", command );

            if ( strcmp( command, "parks") == 0 ) {
                printf( "%s\n", user_input );
                sortParks( catalog, idComp );
                // fprintf( stderr, ""[])
                // for ( int i = 0; i < catalog->count; i++ ) {
                //     fprintf( stderr, "%d\n", catalog->list[ i ]->id );
                // }
                listParks( catalog, testTrue, NULL );
                //fprintf( stderr, "%s\n", "HERE___----__----_-_" );
            }
            else if ( strcmp( command, "county") == 0 ) {
                printf( "%s\n", user_input );
                char county[ COUNTY_NAME_LENGTH + 1 ];
                
                if ( sscanf( user_input + n, "%s", county ) != 1 ) {
                    fprintf( stderr, "%s\n", "Invalid command" );
                    continue;
                }

                sortParks( catalog, nameComp );
                listParks( catalog, testCounty, county );
            }
            else if ( strcmp( command, "names") == 0 ) {
                printf( "%s\n", user_input );
                sortParks( catalog, nameComp );
                listParks( catalog, testTrue, NULL );
            }
        }
        else if ( strcmp( command, "add" ) == 0 ) {
            int park_id = 0;
            sscanf( user_input + n, "%d%n", &park_id, &n );

            // Search for the park to add from the catalog.
            Park *add_park = NULL;

            // Throw invalid command error if the id is not an int.
            if ( park_id == -1 ) {
                fprintf( stderr, "%s\n", "Invalid command" );
                continue;
            }

            for ( int i = 0; i < catalog->count; i++ ) {
                if ( catalog->list[ i ]->id == park_id ) {
                    add_park = catalog->list[ i ];
                }
            }

            // Thow invalid command error if the id does not match an existing park id.
            if ( add_park == NULL ) {
                printf( "%s\n", user_input );
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
        else if ( strcmp( command, "remove" ) == 0 ) {
            int park_id = -1;
            sscanf( user_input + n, "%d%n", &park_id, &n );
            // Park to remove.
            bool remove_park = false;

            // Throw invalid command error if the id is not an int.
            // What if this is a double??? ___---___----_____------______-------_____-------______------______------____-
            if ( park_id == -1 ) {
                fprintf( stderr, "%s\n", "Invalid command" );
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

            // Thow invalid command error if the id does not match an existing park id.
            if ( remove_park == false ) {
                printf( "%s\n", user_input );
                printf( "%s\n\n", "Invalid command" );
                continue;
            }

            printf( "%s\n", user_input );
        }
        else if ( strcmp( command, "trip" ) == 0 ) {
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
        else {
            // Print this message and ask for another command if an invalid command is entered.
            printf( "%s\n", user_input );
            printf( "%s\n", "Invalid command" );
        }

        free( user_input );
        user_input = NULL;
        printf( "%s", "\n" );

        //fprintf( stderr, "%s\n", "HERE___----__----_-_" );
    }
}
