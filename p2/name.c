/**
 * @file name.c
 * @author Ian M Brain (imbrain)
 * This file provides functionality to read in the individuals name and format it correctly.
 * The correct format for a name is lastname, firstname.
 * These two functions are used in formatter.c in order to read and format user input.
  */

#include "name.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/** Exit status for a missing or badly formatted name. */
#define NAME_ERROR 101

/** Two characters that represent the space and comma inbetween the first and last name. */
#define COMMA_SPACE 2

/** One character that represents the space between the first and last name. */
#define SPACE_INDEX 1

/**
 * Reads in the name from standard input and stores it in the parameter string
 * Returns false if end of file is reached and there is not text.
 * Exits with 101 status if the name is longer than 30 characters or does not exist.
 * @param name Variable to store the name within.
 * @return False if end of file is reached and there is no name field. Exit with 101 status error if the name is empty or too long. Return true otherwise. 
*/
bool read_name( char name[ FIELD_MAX + 1 ] )
{
  // Character that has been read in.
  int read_char = getchar();

  // Index of the current character to read.
  int char_index = 0;
  
  while ( read_char != EOF ) {
    if ( char_index > FIELD_MAX ) {
      exit( NAME_ERROR );
    }

    if ( read_char == ':' || read_char == '\n' ) {
      break;
    }

    name[ char_index ] = read_char;
    char_index++;
    name[ char_index ] = '\0';
    read_char = getchar();
  }

  // Return false if the character read in is end of file and no other character has been read.
  if ( char_index == 0 ) {
    return false;
  }
  else if ( strlen( name ) == 0 ) {
    exit( NAME_ERROR );
  }

  return true;
}

/**
 * Transforms the parameter name into the correct format.
 * Capitalizes the first letter of the first and last name.
 * Reverse the order of the names and add a comma between them if there is not a comma separating the names.
 * Exits with 101 status if the name contains non characters besides a comma and a space or contains spaces within names.
 * @param name Name to be fixed.
*/
void fix_name( char name[ FIELD_MAX + 1 ] )
{
  // Length of the string.
  int length = strlen( name );

  // Count of commas in the name.
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

    name[ skip_letters( name, 0 ) + COMMA_SPACE ] = toupper( name[ skip_letters( name, 0 ) + COMMA_SPACE ] );

    // First name field.
    char first_name[ FIELD_MAX + 1 ] = "";

    // Last name field.
    char last_name[ FIELD_MAX + 1 ] = "";

    // Index of the space between the names.
    int space_index = skip_letters( name, 0 ) + SPACE_INDEX;

    strncpy( first_name, name, skip_letters( name, 0 ) );
    copy_substring( last_name, 0, name, ( space_index + SPACE_INDEX ), strlen( name ) );

    // Exit the program if either name does not exist.
    if ( strlen( first_name ) == 0 || strlen( last_name ) == 0 ) {
      exit( NAME_ERROR );
    }

    // Exit the program if the first name contains spaces within it.
    for (int i = 0; first_name[ i ]; i++ ) {
      if ( isspace( last_name[ i ] ) )
        exit( NAME_ERROR );
    }

    // Exit the program if the last name contains spaces within it.
    for (int i = 0; last_name[ i ]; i++ ) {
      if ( isspace( last_name[ i ] ) )
        exit( NAME_ERROR );
    }
  }
  // Switch the first and last name and add a comma if the name does not have a comma.
  else if ( comma_count == 0 ) {
    // Index of the space between the first and last name.
    int space_index = 0;
    space_index = skip_letters( name, 0 );

    // First name field.
    char first_name[ FIELD_MAX + 1 ] = "";

    // Last name field.
    char last_name[ FIELD_MAX + 1 ] = "";

    // Copy the first and last names to the respective fields.
    strncpy( first_name, name, skip_letters( name, 0 ) );
    first_name[ 0 ] = toupper( first_name[ 0 ] );
    copy_substring( last_name, 0, name, ( space_index + SPACE_INDEX ), strlen( name ) );
    last_name[ 0 ] = toupper( last_name[ 0 ] );

    // Exit the program if the first name contains spaces within it.
    for (int i = 0; first_name[ i ]; i++ ) {
      if ( isspace( last_name[ i ] ) )
        exit( NAME_ERROR );
    }

    // Exit the program if the last name contains spaces within it.
    for (int i = 0; last_name[ i ]; i++ ) {
      if ( isspace( last_name[ i ] ) )
        exit( NAME_ERROR );
    }

    // Exit the program if either name does not exist.
    if ( strlen( first_name ) == 0 || strlen( last_name ) == 0 ) {
      exit( NAME_ERROR );
    }

    strcat( last_name, ", " );
    strcat( last_name, first_name );
    strcpy( name, last_name );

    // Exit the program if the name is over the field width after formatting it.
    if ( strlen( name ) > FIELD_MAX ) {
      exit( NAME_ERROR );
    }
  }
  // If the name contains more than one comma, exit the program.
  else {
    exit( NAME_ERROR );
  }
  // NEED TO EXIT WITH ERROR IF IMPROPER FORMAT
}
