
// Include its own header as well,
#include "catalog.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_PARK_COUNTIES 5 

#define INITIAL_CATALOG_PARKS 5

#define RADIUS_EARTH 3959

#define DEG_TO_RAD ( M_PI / 180 )

// CITE THIS_______---------__________----------_________----------__________-----__--
double distance( Park const *a, Park const *b ) {
    int a_lon = a->lon;
    int a_lat = a->lat;
    int b_lon = b->lon;
    int b_lat = b->lon;

    // OK, pretend the center of the earth is at the origin, turn these
    // two locations into vectors pointing from the origin.
    // This could be simplified.
    double v1[] = { cos( a_lon * DEG_TO_RAD ) * cos( a_lat * DEG_TO_RAD ),
                    sin( a_lon * DEG_TO_RAD ) * cos( a_lat * DEG_TO_RAD ),
                    sin( a_lat * DEG_TO_RAD ) };

    double v2[] = { cos( b_lon * DEG_TO_RAD ) * cos( b_lat * DEG_TO_RAD ),
                    sin( b_lon * DEG_TO_RAD ) * cos( b_lat * DEG_TO_RAD ),
                    sin( b_lat * DEG_TO_RAD ) };

    // Dot product these two vectors.
    double dp = 0.0;
    for ( int i = 0; i < sizeof( v1 ) / sizeof( v1[ 0 ] ); i++ )
    dp += v1[ i ] * v2[ i ];

    // Compute the angle between the vectors based on the dot product.
    double angle = acos( dp );

    // Return distance based on the radius of the earth.
    return RADIUS_EARTH * angle;
}

// Still need to allocate memory for each park
Catalog *makeCatalog() {
    Catalog *return_catalog = (Catalog *) malloc( sizeof( Catalog ) );
    return_catalog->list = (Park **) malloc( INITIAL_CATALOG_PARKS * sizeof( Park * ) );
    return_catalog->count = 0;
    return_catalog->capacity = INITIAL_CATALOG_PARKS;

    return return_catalog;
}

// This function frees the memory used to store the given Catalog, including freeing space for all the 
// Parks, freeing the resizable array of pointers and freeing space for the Catalog struct itself.
void freeCatalog( Catalog *catalog ) {
    for ( int i = 0; i < catalog->count; i++ ) {
        free( catalog->list[ i ] );
    }

    free( catalog->list );
    free( catalog );
}

// This function reads all the parks from a park file with the given name. 
// It makes an instance of the Park struct for each one and stores a pointer to that Park in the resizable array in catalog.
void readParks( char const *filename, Catalog *catalog ) {
    FILE *park_file = fopen( filename, "r" );

    // Print file open error and exit the program if the park file cannot be opened.
    if ( !park_file ) {
        fprintf( stderr, "%s%s", "Can't open file: ", filename );
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
    //28 + 60 + 1
    // may need to change this to make it more like command in parks.c
    char *park_info = NULL;
    park_info = readLine( park_file );
    //fprintf( stderr, "%s\n", park_info );

    while ( park_info != NULL ) {
        // char *park_name[ PARK_NAME_LENGTH + 1 ] = readLine( park_file );
        // May need to check for the length elsewhere ____-------_______-------____---_-_-_-_-_-_-----___--
        // Can check if not read correctly by setting it to NULL first
        char *park_name = readLine( park_file );

        // Should this be a pointer? I think so to malloc
        // do we need to initialize this?
        Park *cur_park = (Park *) malloc( sizeof( Park ) );

        // Set each of the values of the park based on the file string.
        int n = 0;
        int num_county = 0;
        // Print invalid file error if the line is missing a field.
        if ( sscanf( park_info, "%d%lf%lf%n", &cur_park->id, &cur_park->lat, &cur_park->lon, &n ) != 3 ) {
            fprintf( stderr, "%s%s", "Invalid park file: ", filename );
            exit( EXIT_FAILURE );
        }
        // Read in parks and identify how many there are
        // Could be done using a while loop with scanf, so while == 1 do this and ++ the county numCounties

        // n_decrease ensures that n does not decrease after reading the entire park_info string.
        int n_decrease = n;
        while ( n >= n_decrease ) {
            n_decrease = n;

            sscanf( ( park_info + n ), "%s%n", cur_park->counties[ num_county ], &n );
            // Print invalid file error if a county name is too long.
            if ( cur_park->counties[ num_county ][ COUNTY_NAME_LENGTH ] != '\0' ) {
                fprintf( stderr, "%s%s", "Invalid park file: ", filename );
                exit( EXIT_FAILURE );
            }

            // Print invalid file error if there are more counties than allowed.
            if ( num_county > 5 ) {
                fprintf( stderr, "%s%s", "Invalid park file: ", filename );
                exit( EXIT_FAILURE );
            }
            // fprintf( stderr, "%s\n", cur_park->counties[ num_county ] );
            num_county++;
        }

        // Print invalid file error if the park does not contain any counties.
        if ( num_county == 0 ) {
            fprintf( stderr, "%s%s", "Invalid park file: ", filename );
            exit( EXIT_FAILURE );
        }

        sscanf( park_name, "%s", cur_park->name );

        // Print invalid file error is the park name is too long.
        if ( cur_park->name[ PARK_NAME_LENGTH ] == '\n' ) {
            fprintf( stderr, "%s%s", "Invalid park file: ", filename );
            exit( EXIT_FAILURE );
        }

        // Add park to the catalog.
        // cur_park is a pointer which the address in list can point to. This memory should be freed later.
        if ( catalog->count >= catalog->capacity ) {
            catalog->capacity *= 2;
            catalog->list = (Park **) realloc( catalog->list, catalog->capacity * sizeof( Park * ) );
        }
        catalog->list[ catalog->count ] = cur_park;
        catalog->count++;

        // Free memory
        free( park_info );
        free( park_name );
        //free( cur_park );

        park_info = NULL;
        park_info = readLine( park_file );
    }

    // Print invlaid file error is two parks have the same id.
    for ( int i = 0; i < catalog->count; i++ ) {
        for ( int j = i + 1; j < catalog->count; j++ ) {
            if ( catalog->list[ i ]->id == catalog->list[ j ]->id ) {
                fprintf( stderr, "%s%s", "Invalid park file: ", filename );
                exit( EXIT_FAILURE );
            }
        }
    }

    fclose( park_file );

}

// This function sorts the parks in the given catalog. 
// It uses the qsort() function together with the function pointer parameter to order the parks. The function pointer is described in the “Sorting Parks” section below.
void sortParks( Catalog *catalog, int (* compare) ( void const *va, void const *vb ) ) {
    // Convert void pointers to the right type
    // Then use them to access fields in the park struct and decide how they compare

    // Should sizeof be park * pointer?
    // Is list the first item? or should list be dereferenced to the first pointer _____-----_____-----______----____-----
    qsort( catalog->list, catalog->count, sizeof( catalog->list[ 0 ] ), compare );
}

// This function prints all or some of the parks. It uses the function pointer parameter together with 
// the string, str, which is passed to the function, to decide which parks to print. 
// This function will be used for the list parks, list names, and list county commands. The function pointer is described in the “Listing Parks” section below.
void listParks( Catalog *catalog, bool (*test)( Park const *park, char const *str ), char const *str ) {
    char park_counties[ MAX_PARK_COUNTIES * COUNTY_NAME_LENGTH + 1 ] = "";

    printf( "%s", "ID  Name                                          Lat      Lon Counties\n");

    for ( int i = 0; i < catalog->count; i++ ) {
        if ( test( catalog->list[ i ], str ) ) {
             // Convert parks counties into the parks separated by a comma.
            strcat( park_counties, catalog->list[ i ]->counties[ 0 ] );
            for ( int c = 1; c < catalog->list[ i ]->numCounties; i++ ) {
                strcat( park_counties, "," );
                strcat( park_counties, catalog->list[ i ]->counties[ c ] );
            }

            if ( test( catalog->list[ i ], str ) ) {
                printf( "%3d ", catalog->list[ i ]->id );
                printf( "%40s ", catalog->list[ i ]->name );
                printf( "%-5.3lf", catalog->list[ i ]->lat );
                printf( "%-5.3lf", catalog->list[ i ]->lon );
                printf( "%s\n", park_counties);
            }
        }
    }
}
