
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <input.h>

#define PARK_NAME_LENGTH 40

#define COUNTY_NAME_LENGTH 12

#define MAX_PARK_COUNTIES 5 

#define INITIAL_CATALOG_PARKS 5

struct ParkStruct {
    int id;
    char name[ PARK_NAME_LENGTH + 1 ];
    double lat;
    double lon;
    int numCounties;
    char counties[ 5 ][ COUNTY_NAME_LENGTH + 1 ];
};

typedef struct ParkStruct Park;

// Array has initial capacity of 5.
struct CatalogStruct {
    Park **list;
    int count;
    int capacity; 
};

typedef struct CatalogStruct Catalog;

double distance( Park const *a, Park const *b ) {

}

// Still need to allocate memory for each park
Catalog *makeCatalog() {
    Catalog *return_catalog = (Catalog *) malloc( sizeof( Catalog ) );
    return_catalog->list = (Park **) malloc( INITIAL_CATALOG_PARKS * sizeof( Park ) );
    return_catalog->count = 0;
    return_catalog->capacity = INITIAL_CATALOG_PARKS;

    return return_catalog;
}

void freeCatalog( Catalog *catalog ) {

}

// This function reads all the parks from a park file with the given name. 
// It makes an instance of the Park struct for each one and stores a pointer to that Park in the resizable array in catalog.
void readParks( char const *filename, Catalog *catalog ) {
    FILE *park_file = fopen( *filename, "r" );

    // Print file open error and exit the program if the park file cannot be opened.
    if ( !park_file ) {
        fprintf( stderr, "%s%s", "Can't open file: ", *filename );
        exit( EXIT_FAILURE );
    }

    // Read in a line
    // Create a park variable and malloc
    // Check that all the variables in the park are correct/throw errors
    // Link park to the catalog
    // make sure to free the memory of the line after each go?
    // Check for errors

    // how long should this be?
    //char field[ 12 + 1 ] = "";
    // At this length, it may prevent the last county from being error checked
    char *park_info[ 28 + 60 + 1 ] = readline( park_file );
    char *park_name[ PARK_NAME_LENGTH + 1 ] = readline( park_file );

    // Should this be a pointer? I think so to malloc
    // do we need to initialize this?
    Park *cur_park = (Park *) malloc( sizeof( Park ) );

    // Set each of the values of the park based on the file string.
    int n = 0;
    int num_county = 0;
    sscanf( *park_info, "%d%lf%lf%n", &cur_park->id, &cur_park->lat, &cur_park->lon, &n );
    // Read in parks and identify how many there are
    // Could be done using a while loop with scanf, so while == 1 do this and ++ the county numCounties
    while ( sscanf( *( park_info + n ), "%s", cur_park->counties[ num_county ] ) == 1 ) {
        // Print invalid file error if a county name is too long.
        if ( cur_park->counties[ num_county ][ COUNTY_NAME_LENGTH ] != '\0' ) {
            fprintf( stderr, "%s%s", "Invalid park file: ", *filename );
            exit( EXIT_FAILURE );
        }

        // Print invalid file error if there are more counties than allowed.
        if ( num_county > 5 ) {
            fprintf( stderr, "%s%s", "Invalid park file: ", *filename );
            exit( EXIT_FAILURE );
        }
        num_county++;
    }

    sscanf( *park_name, "%s", cur_park->name );

    // free memory of read line
}

void sortParks( Catalog *catalog, int (* compare) (void const *va, void const *vb )) {

}

void listParks( Catalog *catalog, bool (*test)( Park const *park, char const *str ), char const *str ) {

}
