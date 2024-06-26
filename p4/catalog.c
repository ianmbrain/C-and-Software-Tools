/**
 * @file catalog.c
 * @author Ian M Brain (imbrain)
 * This file provides the main functionality for the program.
 * THis functionality includes determining the distance between parks, allocating memory to the catalog,
 * reading parks from a file and referencing them in the catalog, sorting the parks, and printing the parks.
 * Uses functionality from input.c to read in lines from a park file to create parks.
 * This functionality is used in parks.c to provide functionality related to parks and the catalog.
  */

#include "catalog.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/** Initial capacity of parks in the catalog. */
#define INITIAL_CATALOG_PARKS 5

/** Radius of the earth in miles. This value is based on the radius from exercise 14. */
#define RADIUS_EARTH 3959

/** Return value of sscanf when reading id, longitude, and latitude. */
#define READ_LINE_VALUES 3

/** Multiplier for converting degrees to radians. This value is based on the value from exercise 14. */
#define DEG_TO_RAD ( M_PI / 180 )

double distance( Park const *a, Park const *b ) {
    double a_lon = a->lon;
    double a_lat = a->lat;
    double b_lon = b->lon;
    double b_lat = b->lat;

    double v1[] = { cos( a_lon * DEG_TO_RAD ) * cos( a_lat * DEG_TO_RAD ),
                    sin( a_lon * DEG_TO_RAD ) * cos( a_lat * DEG_TO_RAD ),
                    sin( a_lat * DEG_TO_RAD ) };

    double v2[] = { cos( b_lon * DEG_TO_RAD ) * cos( b_lat * DEG_TO_RAD ),
                    sin( b_lon * DEG_TO_RAD ) * cos( b_lat * DEG_TO_RAD ),
                    sin( b_lat * DEG_TO_RAD ) };

    double dp = 0.0;
    for ( int i = 0; i < sizeof( v1 ) / sizeof( v1[ 0 ] ); i++ )
    dp += v1[ i ] * v2[ i ];

    double angle = acos( dp );

    return RADIUS_EARTH * angle;
}

Catalog *makeCatalog() {
    Catalog *return_catalog = ( Catalog * ) malloc( sizeof( Catalog ) );
    return_catalog->list = ( Park ** ) malloc( INITIAL_CATALOG_PARKS * sizeof( Park * ) );
    return_catalog->count = 0;
    return_catalog->capacity = INITIAL_CATALOG_PARKS;

    return return_catalog;
}

void freeCatalog( Catalog *catalog ) {
    for ( int i = 0; i < catalog->count; i++ ) {
        free( catalog->list[ i ] );
    }

    free( catalog->list );
    free( catalog );
}

void readParks( char const *filename, Catalog *catalog ) {
    // Opens the file with the specified file name.
    FILE *park_file = fopen( filename, "r" );

    // Print file open error and exit the program if the park file cannot be opened.
    if ( !park_file ) {
        fprintf( stderr, "%s%s\n", "Can't open file: ", filename );
        freeCatalog( catalog );

        exit( EXIT_FAILURE );
    }

    // Read line from the park file containing the information of the park.
    char *park_info = NULL;
    park_info = readLine( park_file );

    // Continue to read in parks from the park file if there are new lines to read.
    while ( park_info != NULL ) {
        // Read line from the park file containing the name of the park.
        char *park_name = NULL;
        park_name = readLine( park_file );

        // Allocates memory for and sets a pointer to a new park.
        Park *cur_park = ( Park * ) malloc( sizeof( Park ) );
        // Initialize park fields that are strings.
        for ( int i = 0; i < MAX_PARK_COUNTIES; i++ ) {
            strcpy( cur_park->counties[ i ], "" );
        }
        strcpy( cur_park->name, "" );

        // Tracks the where in a string has been read to.
        int n = 0;
        // Number of counties in the park
        int num_county = 0;

        // Print invalid file error if the line is missing a field and free memory used for the catalog and park info and name.
        if ( sscanf( park_info, "%d%lf%lf%n", &cur_park->id, &cur_park->lat, &cur_park->lon, &n ) != READ_LINE_VALUES ) {
            fprintf( stderr, "%s%s\n", "Invalid park file: ", filename );
            free( park_info );
            free( park_name );
            free( cur_park );
            freeCatalog( catalog );
            fclose( park_file );

            exit( EXIT_FAILURE );
        }

        // Tracks how many characters were read in the string.
        int add_n = 0;

        // Read each county in the park info string and throw and invalid file error if the format is not valid.
        while ( sscanf( ( park_info + n ), "%s%n", cur_park->counties[ num_county ], &add_n ) == 1 ) {
            // Increment n by the number of characters read in the string.
            n += add_n;

            // Print invalid file error if a county name is too long.
            if ( cur_park->counties[ num_county ][ strlen( cur_park->counties[ num_county ] ) ] != '\0' ) {
                fprintf( stderr, "%s%s\n", "Invalid park file: ", filename );
                free( park_info );
                free( park_name );
                free( cur_park );
                freeCatalog( catalog );
                fclose( park_file );

                exit( EXIT_FAILURE );
            }

            num_county++;
            char county_overflow[ COUNTY_NAME_LENGTH ] = "";

            // Print invalid file error if there are more counties than allowed.
            if ( num_county == MAX_PARK_COUNTIES ) {
                if ( sscanf( ( park_info + n ), "%s%n", county_overflow, &add_n ) == 1) {
                    fprintf( stderr, "%s%s\n", "Invalid park file: ", filename );
                    free( park_info );
                    free( park_name );
                    free( cur_park );
                    freeCatalog( catalog );
                    fclose( park_file );

                    exit( EXIT_FAILURE );
                }
            }

            // Print invalid file error if there are more counties than allowed.
            if ( num_county > MAX_PARK_COUNTIES ) {
                fprintf( stderr, "%s%s\n", "Invalid park file: ", filename );
                free( park_info );
                free( park_name );
                free( cur_park );
                freeCatalog( catalog );
                fclose( park_file );

                exit( EXIT_FAILURE );
            }
        }
        // Set the park number of counties field to the number of counties read in.
        cur_park->numCounties = num_county;

        // Print invalid file error if the park does not contain any counties.
        if ( num_county == 0 ) {
            fprintf( stderr, "%s%s\n", "Invalid park file: ", filename );
            free( park_info );
            free( park_name );
            free( cur_park );
            freeCatalog( catalog );
            fclose( park_file );

            exit( EXIT_FAILURE );
        }

        // Copy the park name to the park name field.
        strcpy( cur_park->name, park_name );

        // Print invalid file error is the park name is too long.
        if ( strlen( cur_park->name ) > PARK_NAME_LENGTH ) {
            fprintf( stderr, "%s%s\n", "Invalid park file: ", filename );
            free( park_info );
            free( park_name );
            free( cur_park );
            freeCatalog( catalog );
            fclose( park_file );
            
            exit( EXIT_FAILURE );
        }

        // Reference the park pointer in the catalog list of parks. Memory for the park is freed at the end of the program. Resize the park list if the count of parks reaches the capacity.
        if ( catalog->count >= catalog->capacity ) {
            catalog->capacity *= DOUBLE_CAPACITY;
            catalog->list = (Park **) realloc( catalog->list, catalog->capacity * sizeof( Park * ) );
        }
        catalog->list[ catalog->count ] = cur_park;
        catalog->count++;

        // Free memroy of the park info and parkname
        free( park_info );
        free( park_name );

        // Read in the next park info text line.
        park_info = NULL;
        park_info = readLine( park_file );
    }

    // Print invlaid file error is two parks have the same id and exit the program.
    for ( int i = 0; i < catalog->count; i++ ) {
        for ( int j = i + 1; j < catalog->count; j++ ) {
            if ( catalog->list[ i ]->id == catalog->list[ j ]->id ) {
                fprintf( stderr, "%s%s\n", "Invalid park file: ", filename );
                freeCatalog( catalog );
                fclose( park_file );

                exit( EXIT_FAILURE );
            }
        }
    }

    fclose( park_file );

}

void sortParks( Catalog *catalog, int (* compare) ( void const *va, void const *vb ) ) {
    qsort( catalog->list, catalog->count, sizeof( Park * ), compare );
}

void listParks( Catalog *catalog, bool (*test)( Park const *park, char const *str ), char const *str ) {
    // Header to print before the parks specifying what each park field represents.
    printf( "%s", "ID  Name                                          Lat      Lon Counties\n");

    for ( int i = 0; i < catalog->count; i++ ) {
        // Print the park if it matches the the string based on the test function.
        if ( test( catalog->list[ i ], str ) ) {
            printf( "%-3d ", catalog->list[ i ]->id );
            printf( "%-40s ", catalog->list[ i ]->name );
            printf( "%8.3lf ", catalog->list[ i ]->lat );
            printf( "%8.3lf ", catalog->list[ i ]->lon );

            // Print out the counties of the park.
            for ( int c = 0; c < catalog->list[ i ]->numCounties; c++ ) {
                // Specifies that parks that are not the last park should be printed with a comma. The last park should have a new line character.
                if ( c < catalog->list[ i ]->numCounties - 1 )
                    printf( "%s,", catalog->list[ i ]->counties[ c ] );
                else
                    printf( "%s\n", catalog->list[ i ]->counties[ c ] );
            }
        }
    }
}
