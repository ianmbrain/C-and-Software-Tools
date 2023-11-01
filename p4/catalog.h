/**
 * @file catalog.h
 * @author Ian M Brain (imbrain)
 * This file provides the main functionality for the program.
 * THis functionality includes determining the distance between parks, allocating memory to the catalog,
 * reading parks from a file and referencing them in the catalog, sorting the parks, and printing the parks.
 * Uses functionality from input.c to read in lines from a park file to create parks.
 * This functionality is used in parks.c to provide functionality related to parks and the catalog.
  */

#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/** Maximum length of a park name. */
#define PARK_NAME_LENGTH 40

/** Maximum length of a county name. */
#define COUNTY_NAME_LENGTH 12

/** Max number of counties a park can be associated with. */
#define MAX_PARK_COUNTIES 5

/**
 * Struct defining a park.
 * Contains fields for the park id, name, latitude, longitude,
 * number of counties, and a list for the number of counties
*/
typedef struct ParkStruct {
    int id;
    char name[ PARK_NAME_LENGTH + 1 ];
    double lat;
    double lon;
    int numCounties;
    char counties[ MAX_PARK_COUNTIES ][ COUNTY_NAME_LENGTH + 1 ];
} Park;

/**
 * Struct defining a catalog.
 * Contains fields for a pointer referencing the array of park pointers,
 * the count of parks in the catalog, and the capacity of parks in the catalog
 * so the array of park pointers can be be resized.
*/
typedef struct CatalogStruct {
    Park **list;
    int count;
    int capacity; 
} Catalog;

/**
 * Compares the distance between two parks using there longitude and latitude values.
 * Parks are referenced as pointers which the longitude and latitude values are gathered from.
 * This function is based on the value from exercise 14 and adapted to this project.
 * @param a first park to compare the coordinates of.
 * @param b second park to compare the coordinates of.
 * @return distance in miles between the two park coordinates.
*/
double distance( Park const *a, Park const *b );

/**
 * Allocates memory to a new catalog and returns a pointer to that memory.
 * Memory is allocated for the catalog struct and the list of parks.
 * @return pointer to a new catalog.
*/
Catalog *makeCatalog();

/**
 * Frees the memory of each park stored in the catalog, the pointer to the array of park pointers, and the catalog itself.
 * Used in parks.c to free the memory of the catalog after finishing user input.
 * @param catalog pointer to the catalog to free from memory.
*/
void freeCatalog( Catalog *catalog );

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
void readParks( char const *filename, Catalog *catalog );

/**
 * Sorts the parks in the catalog based on a comparison function.
 * Utilizes qsort() to sort each of the parks in the catalog.
 * @param catalog catalog containing the list of parks to sort.
 * @param compare function specifying how the parks should be sorted.
*/
void sortParks( Catalog *catalog, int (* compare) ( void const *va, void const *vb ) );

/**
 * Prints out the parks in the catalog to standard output.
 * Prints the park id, name, latitude, longitude, and counties.
 * Only prints out the parks specified by the test function parameter.
 * If printing out parks based on their counties, the str parameter will be the county.
 * @param catalog catalog containing the parks to be printed. 
 * @param test function that determins what parks to be printed.
 * @param str string to compare each park to thereby determining if it should be printed.
*/
void listParks( Catalog *catalog, bool (*test)( Park const *park, char const *str ), char const *str );
