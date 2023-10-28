/**
 * @file catalog.h
 * @author Ian M Brain (imbrain)
  */

#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define PARK_NAME_LENGTH 40

#define COUNTY_NAME_LENGTH 12

// should structs go here?? _____------_____------______-----______------____-
struct ParkStruct {
    int id;
    char name[ PARK_NAME_LENGTH + 1 ];
    double lat;
    double lon;
    int numCounties;
    char counties[ 5 ][ COUNTY_NAME_LENGTH + 1 ];
};

typedef struct ParkStruct Park;

struct CatalogStruct {
    Park **list;
    int count;
    int capacity; 
};

typedef struct CatalogStruct Catalog;

double distance( Park const *a, Park const *b );

Catalog *makeCatalog();

void freeCatalog( Catalog *catalog );

void readParks( char const *filename, Catalog *catalog );

void sortParks( Catalog *catalog, int (* compare) ( void const *va, void const *vb ) );

void listParks( Catalog *catalog, bool (*test)( Park const *park, char const *str ), char const *str );
