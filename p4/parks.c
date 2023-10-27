// Contains Main()
// You can define the comparison functions in your top-level parks component and make them static

#include <catalog.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <input.h>

#define CMD_LENGTH 6

#define INITIAL_TRIP_SIZE 5


static int idComp( void const *va, void const *vb )
{
    Park const *p1 = va;
    Park const *p2 = vb;

    if ( p1->id < p2->id ) {
        return -1;
    }
    else if ( p1->id < p2->id ) {
        return 1;
    }
    else {
        return 0;
    }   
}

static int nameComp( void const *va, void const *vb )
{
    Park const *p1 = va;
    Park const *p2 = vb;

    int str_cmp = strcmp( p1->name, p2->name );
    if ( str_cmp == 0 ) {
        return idComp( va, vb );
    }
    else {
        return str_cmp;
    }
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
    if ( argc == 0 ) {
        fprintf( stderr, "%s", "usage: parks <park-file>*" );
        exit( EXIT_FAILURE );
    }

    // Read parks from inputted files into the catalog.
    for ( int i = 0; i < argc; i++ ) {
        readParks( argv[ i ], catalog );
    }
    

    printf( "%s", "cmd> " );

    char *user_input;
    char command[ CMD_LENGTH + 1 ] = "";

    while( true ) {
        int n = 0;
        user_input = readLine( stdin );

        // Read the command of the user input.
        sscanf( user_input, "%s%n", command, &n );

        if ( strcmp( command, "list" ) == 0 ) {
            sscanf( user_input + n, "%s%n", command, &n );

            if ( strcmp( command, "parks") ) {
                // Print parks ____--___--__-_-_--_-_---
            }
            else if ( strcmp( command, "county") ) {
                char counties[ 12 ][ COUNTY_NAME_LENGTH + 1 ] = {""};
                int county_cnt = 0;
                char county[ COUNTY_NAME_LENGTH + 1 ];
                
                while ( sscanf( user_input + n, "%s%n", county, &n ) == 1 ) {
                    strcpy( counties[ county_cnt ], county );
                }
                // Choose and print parks ____--___--__-_-_--_-_---
            }

            printf( "%s\n", user_input );
        }
        else if ( strcmp( command, "add" ) == 0 ) {
            int park_id = 0;
            sscanf( user_input + n, "%d%n", &park_id, &n );

            // Search for the park to add from the catalog.
            Park *add_park = NULL;

            for ( int i = 0; i < catalog->count; i++ ) {
                if ( catalog->list[ i ]->id == park_id ) {
                    add_park = catalog->list[ i ];
                }
            }

            // Thow invalid command error if the id does not match an existing park id.
            if ( add_park == NULL ) {
                fprintf( stderr, "%s\n", "Invalid command" );
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
            int park_id = 0;
            sscanf( user_input + n, "%d%n", &park_id, &n );
            // Park to remove.
            Park *remove_park = NULL;

            // Search for the park to remove from the trip.
            for ( int i = 0; i < trip->count; i++ ) {
                if ( trip->list[ i ]->id == park_id ) {
                    for ( int j = i; j < trip -> count; j++ ) {
                        //____--_----_-------_______---____--___----___----____----___-
                    }
                }
            }

            printf( "%s\n", user_input );
        }
        else if ( strcmp( command, "trip" ) == 0 ) {

        }
        else if ( strcmp( command, "quit" ) == 0 ) {

        }
        else {
            // Print this message and ask for another command if an invalid command is entered.
            printf( "%s", "Invalid command" );
        }
    }
}
