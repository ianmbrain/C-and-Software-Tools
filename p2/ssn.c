/**
  */

#include "ssn.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/** Exit status for a missing or badly formatted ssn. */
#define SSN_ERROR 103

/** Number of digits in the first group in the SSN. */
#define START_DIGITS 3

/** Number of digits in the middle group in the SSN. */
#define MIDDLE_DIGITS 2

/** Number of digits in the last group in the SSN. */
#define END_DIGITS 4

/**
 * Read in the user inputted ssn to the ssn parameter variable from standard input.
 * Exit with 103 status if the ssn cannot be read or it is too long.
 * @param ssn ssn variable to store the user inputted ssn to.
*/
void read_ssn( char ssn[ FIELD_MAX + 1 ] ) {
  // NEED TO CHANGE 11 TO FIELD_MAX
  // if ( scanf( "%11s", ssn ) == -1 ) {
  //   exit( SSN_ERROR );
  // }

  // if ( strlen( ssn ) != ( START_DIGITS + MIDDLE_DIGITS + END_DIGITS ) || strlen( ssn ) != ( START_DIGITS + MIDDLE_DIGITS + END_DIGITS + 2 ) || strcmp( ssn, "N/A" ) != 0 ) {
  //   exit( SSN_ERROR );
  // }

  int read_char = getchar();
  int char_index = 0;
  
  while ( read_char != EOF ) {
    if ( char_index > FIELD_MAX ) {
      exit( SSN_ERROR );
    }
    
    if ( read_char == '\n' ) {
      break;
    }

    ssn[ char_index ] = read_char;
    char_index++;
    ssn[ char_index ] = '\0';
    read_char = getchar();
  }

  if ( strlen( ssn ) == 0 /** || char_index > FIELD_MAX */ ) {
    exit( SSN_ERROR );
  }

  int digit_count;
  for ( int i = 0; ssn[ i ]; i++ ) {
    if ( isdigit( ssn[ i ] ) )
      digit_count++;
  }

  if ( digit_count > ( START_DIGITS + MIDDLE_DIGITS + END_DIGITS ) )
    exit( SSN_ERROR );
}

/**
 * Updates the ssn to the format of three digits, dash, two digits, dash, final four digits.
 * Exit with 103 status if the ssn is not in one of three formats:
 * three digits, dash, two digits, dash, final four digits,
 * nine consecutive digits,
 * or N/A.
 * @param ssn ssn to update to the correct format. 
*/
void fix_ssn( char ssn[ FIELD_MAX + 1 ] ) {
  int first_non_digit = skip_digits( ssn, 0 );

  // If the index of a non digit is 0 then the format should be N/A.
  if ( first_non_digit == 0 ) {
    if ( strcmp( ssn, SSN_NA ) != 0 )
      exit( SSN_ERROR );
  }
  // If the non digit index indicates the ssn is already in standard format.
  else if ( first_non_digit == 3 ) {
    if ( ssn[ first_non_digit ] != '-' || ssn[ skip_digits( ssn, first_non_digit + 1 ) ] != '-' )
      exit( SSN_ERROR );
  }
  // Otherwise the ssn should be in the format of just nine numbers. Change this ssn to standard format.
  else {
    char cpy_ssn[ FIELD_MAX + 1 ] = "";
    int cpy_index = 0;

    copy_substring( cpy_ssn, cpy_index, ssn, 0, START_DIGITS );
    cpy_index += START_DIGITS;

    copy_substring( cpy_ssn, cpy_index, "-", 0, 1 );
    cpy_index++;

    copy_substring( cpy_ssn, cpy_index, ssn, START_DIGITS, ( START_DIGITS + MIDDLE_DIGITS ) );
    cpy_index += MIDDLE_DIGITS;

    copy_substring( cpy_ssn, cpy_index, "-", 0, 1 );
    cpy_index++;

    copy_substring( cpy_ssn, cpy_index, ssn, ( START_DIGITS + MIDDLE_DIGITS ), ( START_DIGITS + MIDDLE_DIGITS + END_DIGITS ) );

    strcpy( ssn, cpy_ssn );
  }
}