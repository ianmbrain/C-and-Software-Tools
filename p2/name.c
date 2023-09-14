/**
  */

#include "name.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/** Exit status for a missing or badly formatted name. */
#define NAME_ERROR 101

bool read_name( char name[ FIELD_MAX + 1 ] )
{
  // Return false if end of file is reached or there is no name field.
  // MAYBE MAKE 30 FIELD_MAX
  if( scanf( "%30s", name ) == -1 ) {
    return false;
  }

  if( strlen( name ) == 0 || strlen( name ) > 30 ) {
    exit( NAME_ERROR );
  }
}

void fix_name( char name[ FIELD_MAX + 1 ] )
{
  // Exit with status of 101 if the name isn't in an acceptable format.
  // If the name contains a comma, it is formatted correctly. If the name does not, the last name should be swapped with the first name and a comma added.
  // This RELIES ON UTIL. I should start at util.c.

}
