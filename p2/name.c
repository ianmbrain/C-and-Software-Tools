/**
  */

#include "name.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/** Exit status for a missing or badly formatted name. */
#define NAME_ERROR 101

/**
 * Reads in the name from standard input and stores it in the parameter string
 * Returns false if end of file is reached and there is not text.
 * Exits with 101 status if the name is longer than 30 characters or does not exist.
 * @param name Variable to store the name within.
 * @return False if end of file is reached and there is no name field. Exit with 101 status error if the name is empty or too long. Return true otherwise. 
*/
bool read_name( char name[ FIELD_MAX + 1 ] )
{
  // NEED TO CHANGE 30 TO FIELD_MAX
  if ( scanf( "%30s", name ) == -1 ) {
    return false;
  }

  if ( strlen( name ) == 0 || strlen( name ) > 30 ) {
    exit( NAME_ERROR );
  }

  return true;
}

/**
 * Transforms the parameter name into the correct format.
 * Capitalizes the first letter of the first and last name.
 * Reverse the order of the names and add a comma between them if there is not a comma separating the names.
 * Exits with 101 status if the name contains non characters besides a comma and a space.
 * @param name Name to be fixed.
*/
void fix_name( char name[ FIELD_MAX + 1 ] )
{
  int length = strlen( name );
  int comma_count = 0;

  for ( int i = 0; i < length; i++ ) {
    if ( name[ i ] == ',' )
      comma_count++;
    else if ( !isalpha( name[ i ] ) && !isspace( name[ i ] ) )
      exit( NAME_ERROR );
  }

  // If the string is formatted with a comma, capitalize the first letter of the first and last name.
  if ( comma_count == 1 ) {
    name[ 0 ] = toupper( name[ 0 ] );

    name[ skip_letters( name, 0 ) + 2 ] = toupper( skip_letters( name, 0 ) + 2 );
  }
  else if ( comma_count == 0 ) {
    // DOES THIS ARRAY NEED TO BE RESIZED? NO AS FIELD_MAX + 1 FOR NULL TERMINATOR
    int space_index = skip_letters( name, 0 );

    char first_name[ FIELD_MAX + 1 ] = "";
    char last_name[ FIELD_MAX + 1 ] = "";

    strncpy( first_name, name, 0 );
    first_name[ 0 ] = toupper( first_name[ 0 ] );
    strncpy( last_name, name, space_index + 1 );
    last_name[ 0 ] = toupper( last_name[ 0 ] );

    strcat( first_name, ", " );
    strcat( first_name, last_name );
    strcat( name, first_name );
  }
  else {
    exit( NAME_ERROR );
  }
  // NEED TO EXIT WITH ERROR IF IMPROPER FORMAT
}
