/**
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

/**
 * Reads in the date from standard input and exits with 102 status if the name does not exist or is not 8 or 10 characters in length.
 * @param date Date variable to store the inputted date in.
*/
void read_date( char date[ FIELD_MAX + 1 ] ) {
  // NEED TO CHANGE 11 TO FIELD_MAX
  // if ( scanf( "%11s", date ) == -1 ) {
  //   exit( DATE_ERROR );
  // }

  // if ( strlen( date ) != ( SHORT_YEAR + MONTH_DIGITS + DAY_DIGITS ) || strlen( date ) != ( FULL_YEAR + MONTH_DIGITS + DAY_DIGITS ) ) {
  //   exit( DATE_ERROR );
  // }

  int read_char = getchar();
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

  if ( strlen( date ) == 0 /** || char_index > FIELD_MAX */ ) {
    exit( DATE_ERROR );
  }
}

/**
 * Transforms the date into the proper YYYY-MM-DD format.
 * Exit with 102 status if the date is not in one of the specified formats.
 * @param date Date variable to be transformed.
*/
void fix_date( char date[ FIELD_MAX + 1 ] ) {
  int first_sep = skip_digits( date, 0 );

  if ( first_sep == 4 ) {
    if ( date[ 4 ] != '-' || date[ 7 ] != '-' )
      exit( DATE_ERROR );

    // Check if any characters that should be digits are not digits.
    for ( int i = 0; i < ( FULL_YEAR + MONTH_DIGITS + DAY_DIGITS + 2 ); i++ ) {
      if ( i != 4 && i != 7 ) {
        if ( !isdigit( date [ i ] ) ) {
          exit( DATE_ERROR );
        }
      }
    }
    
  }
  else if ( first_sep == 2 ) {
    char year[ FIELD_MAX + 1 ] = "";
    char day[ 3 ] = "";
    char month [ 3 ] = "";

    if ( date[ 2 ] == '-' ) {
      if ( date[ 5 ] != '-' )
        exit( DATE_ERROR );
    }
    else if ( date[ 2 ] == '/' ) {
      if ( date[ 5 ] != '/' )
        exit( DATE_ERROR );
    }
    else {
      exit( DATE_ERROR );
    }

    // Check if any characters that should be digits are not digits.
    for ( int i = 0; i < ( SHORT_YEAR + MONTH_DIGITS + DAY_DIGITS + 2 ); i++ ) {
      if ( i != 2 && i != 5 ) {
        if ( !isdigit( date [ i ] ) ) {
          exit( DATE_ERROR );
        }
      }
    }

    strncpy( day, date, 2 );
    month[ 0 ] = date[ 3 ];
    month[ 1 ] = date[ 4 ];

    if ( strlen( date ) == ( SHORT_YEAR + MONTH_DIGITS + DAY_DIGITS + 2 ) ) {
      year[ 2 ] = date[ 6 ];
      year[ 3 ] = date[ 7 ];

      if ( year[ 2 ] < CURRENT_YEAR[ 0 ] ) {
        year[ 0 ] = '2';
        year[ 1 ] = '0';
      }
      else if ( year[ 2 ] == CURRENT_YEAR[ 0 ] ) {
        if ( year[ 3 ] <= '3' ) {
          year[ 0 ] = '2';
          year[ 1 ] = '0';
        }
      }
      else {
        year[ 0 ] = '1';
        year[ 1 ] = '9';
      }
    }
    else if ( strlen( date ) == ( FULL_YEAR + MONTH_DIGITS + DAY_DIGITS + 2 ) ) {
      year[ 0 ] = date[ 6 ];
      year[ 1 ] = date[ 7 ];
      year[ 2 ] = date[ 8 ];
      year[ 3 ] = date[ 9 ];
    }

    strcat( year, "-" );
    strcat( year, day );
    strcat( year, "-" );
    strcat( year, month );
    strcpy( date, year );
  }
  else {
    exit( DATE_ERROR );
  }
}
