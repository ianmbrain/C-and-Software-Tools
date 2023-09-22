/**
 * @file date.c
 * @author Ian M Brain (imbrain)
 * This file provides functionality to read in the individuals phone number and format it correctly.
 * The correct format for a phone number is YYYY-MM-DD.
 * These two functions are used in formatter.c in order to read and format user input.
  */

#include "date.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/** Exit status for a missing or badly formatted date. */
#define DATE_ERROR 102

/** Two low-order digits of the current year. */
#define CURRENT_YEAR "23"

/** Number of digits in the short (2 digit) year. */
#define SHORT_YEAR 2

/** Number of digits in the full (4 digit) year. */
#define FULL_YEAR 4

/** Number of digits in the month. */
#define MONTH_DIGITS 2

/** Number of digits in the day. */
#define DAY_DIGITS 2

/** Length of the date with short year. */
#define SHORT_YEAR_LENGTH 8

/** Length of the date with full year. */
#define FULL_YEAR_LENGTH 10

/** Index of the first separator for dates that start with a full year. */
#define FULL_FIRST_SEPARATOR 4

/** Index of the second separator for dates that start with a full year. */
#define FULL_SECOND_SEPARATOR 7

/** Index of the first separator for dates that do not start with a full year. */
#define SHORT_FIRST_SEPARATOR 2

/** Index of the second separator for dates that do not start with a full year. */
#define SHORT_SECOND_SEPARATOR 5

/** Index of the first digit in a four digit date. */
#define FIRST_YEAR_DIGIT 2

/** Index of the second digit in a four digit date. */
#define SECOND_YEAR_DIGIT 3

/**
 * Reads in the date from standard input and exits with 102 status if the name does not exist or is not 8 or 10 characters in length.
 * @param date Date variable to store the inputted date in.
*/
void read_date( char date[ FIELD_MAX + 1 ] ) {
  // Character that has been read in.
  int read_char = getchar();

  // Index of the current character to read.
  int char_index = 0;
  
  while ( read_char != EOF ) {
    if ( char_index > FIELD_MAX ) {
      exit( DATE_ERROR );
    }
    
    if ( read_char == ':' || read_char == '\n' ) {
      break;
    }

    date[ char_index ] = read_char;
    char_index++;
    date[ char_index ] = '\0';
    read_char = getchar();
  }

  if ( strlen( date ) == 0 ) {
    exit( DATE_ERROR );
  }
}

/**
 * Transforms the date into the proper YYYY-MM-DD format.
 * Exit with 102 status if the date is not in one of the specified formats:
 * YYYY-DD-MM, DD/MM/YYYY, DD/MM/YY, DD-MM-YYYY, DD-MM-YY.
 * @param date Date variable to be transformed.
*/
void fix_date( char date[ FIELD_MAX + 1 ] ) {
  // Index of the first separator in the date.
  int first_sep = skip_digits( date, 0 );

  if ( first_sep == FULL_YEAR ) {
    if ( date[ FULL_FIRST_SEPARATOR ] != '-' || date[ FULL_SECOND_SEPARATOR ] != '-' )
      exit( DATE_ERROR );

    // Check if any characters that should be digits are not digits.
    for ( int i = 0; i < ( FULL_YEAR_LENGTH ); i++ ) {
      if ( i != FULL_FIRST_SEPARATOR && i != FULL_SECOND_SEPARATOR ) {
        if ( !isdigit( date [ i ] ) ) {
          exit( DATE_ERROR );
        }
      }
    }
    
  }
  else if ( first_sep == SHORT_FIRST_SEPARATOR ) {
    char year[ FIELD_MAX + 1 ] = "";
    char day[ FIELD_MAX + 1 ] = "";
    char month [ FIELD_MAX + 1 ] = "";

    if ( date[ SHORT_FIRST_SEPARATOR ] == '-' ) {
      if ( date[ SHORT_SECOND_SEPARATOR ] != '-' )
        exit( DATE_ERROR );
    }
    else if ( date[ SHORT_FIRST_SEPARATOR ] == '/' ) {
      if ( date[ SHORT_SECOND_SEPARATOR ] != '/' )
        exit( DATE_ERROR );
    }
    else {
      exit( DATE_ERROR );
    }

    // Check if any characters that should be digits are not digits.
    for ( int i = 0; i < ( SHORT_YEAR_LENGTH ); i++ ) {
      if ( i != SHORT_FIRST_SEPARATOR && i != SHORT_SECOND_SEPARATOR ) {
        if ( !isdigit( date [ i ] ) ) {
          exit( DATE_ERROR );
        }
      }
    }

    // Set the month values to the respective month from the date.
    strncpy( day, date, SHORT_FIRST_SEPARATOR );

    copy_substring( month, 0, date, SHORT_FIRST_SEPARATOR + 1, SHORT_SECOND_SEPARATOR );

    if ( strlen( date ) == ( SHORT_YEAR_LENGTH ) ) {
      copy_substring( year, FIRST_YEAR_DIGIT, date, SHORT_SECOND_SEPARATOR + 1, SHORT_YEAR_LENGTH );

      if ( year[ FIRST_YEAR_DIGIT ] < CURRENT_YEAR[ 0 ] ) {
        year[ 0 ] = '2';
        year[ 1 ] = '0';
      }
      else if ( year[ FIRST_YEAR_DIGIT ] == CURRENT_YEAR[ 0 ] ) {
        if ( year[ SECOND_YEAR_DIGIT ] <= '3' ) {
          year[ 0 ] = '2';
          year[ 1 ] = '0';
        }
      }
      else {
        year[ 0 ] = '1';
        year[ 1 ] = '9';
      }
    }
    // Use the entire year from the date if the length of the date indicates a full year was used
    else if ( strlen( date ) == ( FULL_YEAR_LENGTH ) ) {
      copy_substring( year, 0, date, SHORT_SECOND_SEPARATOR + 1, FULL_YEAR_LENGTH );
    }

    // Assemble the date in the correct format
    strcat( year, "-" );
    strcat( year, day );
    strcat( year, "-" );
    strcat( year, month );
    strcpy( date, year );
  }
  // Exit the program if the date does not contain a separator at the correct index, indicating it is invalid input.
  else {
    exit( DATE_ERROR );
  }
}
