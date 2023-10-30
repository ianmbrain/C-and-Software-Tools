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

/** Multiplier for converting degrees to radians. This value is based on the value from exercise 14. */
#define DEG_TO_RAD ( M_PI / 180 )

/**
 * Compares the distance between two parks using there longitude and latitude values.
 * Parks are referenced as pointers which the longitude and latitude values are gathered from.
 * This function is based on the value from exercise 14 and adapted to this project.
 * @param a first park to compare the coordinates of.
 * @param b second park to compare the coordinates of.
 * @return distance in miles between the two park coordinates.
*/
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

/**
 * Allocates memory to a new catalog and returns a pointer to that memory.
 * Memory is allocated for the catalog struct and the list of parks.
 * @return pointer to a new catalog.
*/
Catalog *makeCatalog() {
    Catalog *return_catalog = ( Catalog * ) malloc( sizeof( Catalog ) );
    return_catalog->list = ( Park ** ) malloc( INITIAL_CATALOG_PARKS * sizeof( Park * ) );
    return_catalog->count = 0;
    return_catalog->capacity = INITIAL_CATALOG_PARKS;

    return return_catalog;
}

/**
 * Frees the memory of each park stored in the catalog, the pointer to the array of park pointers, and the catalog itself.
 * Used in parks.c to free the memory of the catalog after finishing user input.
 * @param catalog pointer to the catalog to free from memory.
*/
void freeCatalog( Catalog *catalog ) {
    for ( int i = 0; i < catalog->count; i++ ) {
        free( catalog->list[ i ] );
    }

    free( catalog->list );
    free( catalog );
}

/**
 * Reads the parks from the specified file.
 * Parks are read in using the readLine function()
 * Memory is allocated for each park and the pointer to that park is stored in the catalog.
 * Throws a file error and exits the program if the file cannot be opened.
 * Throws an invalid file error and exits the program if the file format is not correct,
 * the park name is to long, the park has too many counties or none, or if a county name is too long.
 * @param filename name of the file to read parks from.
 * @param catalog catalog to store the parks within.
*/
void readParks( char const *filename, Catalog *catalog ) {
    // Opens the file with the specified file name
    FILE *park_file = fopen( filename, "r" );

    // Print file open error and exit the program if the park file cannot be opened.
    if ( !park_file ) {
        fprintf( stderr, "%s%s", "Can't open file: ", filename );
        fprintf( stderr, "%s", "\n" );
        exit( EXIT_FAILURE );
    }

    // Read line from the park file containing the information of the park.
    char *park_info = NULL;
    park_info = readLine( park_file );

    // Continue to read in parks from the park file if there are new lines to read
    while ( park_info != NULL ) {
        // Read line from the park file containing the name of the park.
        char *park_name = NULL;
        park_name = readLine( park_file );

        // Allocates memory for and sets a pointer to a new park.
        Park *cur_park = NULL;
        cur_park = (Park *) malloc( sizeof( Park ) );

        // Tracks the where in a string has been read to.
        int n = 0;
        // Number of counties in the park
        int num_county = 0;

        // Print invalid file error if the line is missing a field and free memory used for the catalog and park info and name.
        if ( sscanf( park_info, "%d%lf%lf%n", &cur_park->id, &cur_park->lat, &cur_park->lon, &n ) != 3 ) {
            fprintf( stderr, "%s%s\n", "Invalid park file: ", filename );
            free( park_info );
            free( park_name );

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
                exit( EXIT_FAILURE );
            }

            num_county++;

            // Print invalid file error if there are more counties than allowed.
            if ( num_county > MAX_PARK_COUNTIES ) {
                fprintf( stderr, "%s%s\n", "Invalid park file: ", filename );
                free( park_info );
                free( park_name );
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
            exit( EXIT_FAILURE );
        }

        // Copy the park name to the park name field.
        strcpy( cur_park->name, park_name );

        // Print invalid file error is the park name is too long.
        if ( strlen( cur_park->name ) > PARK_NAME_LENGTH ) {
            fprintf( stderr, "%s%s\n", "Invalid park file: ", filename );
            free( park_info );
            free( park_name );
            exit( EXIT_FAILURE );
        }

        // Reference the park pointer in the catalog list of parks. Memory for the park is freed at the end of the program. Resize the park list if the count of parks reaches the capacity.
        if ( catalog->count >= catalog->capacity ) {
            catalog->capacity *= 2;
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
                exit( EXIT_FAILURE );
            }
        }
    }

    fclose( park_file );

}

/**
 * Sorts the parks in the catalog based on a comparison function.
 * Utilizes qsort() to sort each of the parks in the catalog.
 * @param catalog catalog containing the list of parks to sort.
 * @param compare function specifying how the parks should be sorted.
*/
void sortParks( Catalog *catalog, int (* compare) ( void const *va, void const *vb ) ) {
    qsort( catalog->list, catalog->count, sizeof( Park * ), compare );
}

/**
 * Prints out the parks in the catalog to standard output.
 * Prints the park id, name, latitude, longitude, and counties.
 * Only prints out the parks specified by the test function parameter.
 * If printing out parks based on their counties, the str parameter will be the county.
 * @param catalog catalog containing the parks to be printed. 
 * @param test function that determins what parks to be printed.
 * @param str string to compare each park to thereby determining if it should be printed.
*/
void listParks( Catalog *catalog, bool (*test)( Park const *park, char const *str ), char const *str ) {
    // Header to print before the parks specifying what each park field represents
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
