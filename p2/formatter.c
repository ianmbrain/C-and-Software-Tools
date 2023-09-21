/**
 * @file formatter.c
 * @author Ian M Brain (imbrain)
 * This file is where the program is run and contains the main function.
 * Formatter uses the read functions from the other files to read user information from standard input.
 * The input values are then fixed and printed.
 * A summary of the individuals is printed as well.
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "name.h"
#include "date.h"
#include "ssn.h"

/** Birthday for people who will be 21 on the day this assignment is due. */
#define AGE_CUTOFF "2002-09-22"

/**
 * This is the program starting point.
 * Reads in individual information from standard input using read functions.
 * Formats these into the correct formats using the fix functions and the results are printed.
 * In addition, summary statistics of the individuals are printed out:
 * the number of individuals, max name length, individuals 21 or older, and individuals without and ssn.
 * If there is any invalid input, the program exits at that line and does not finish.
 * @returns EXIT_SUCCESS if there are no errors or the program is exited before completion.
*/
int main() {
  // Count of the individuals.
  int individual_count = 0;

  // Max name inputted.
  int max_name_size = 0;

  // Number of individuals 21 or older.
  int age_cutoff_count = 0;

  // Count of individuals without an ssn.
  int no_ssn_count = 0;

  char name[ FIELD_MAX + 1 ];
  char date[ FIELD_MAX + 1 ];
  char ssn[ FIELD_MAX + 1 ];

  while ( read_name( name ) != false ) {
    read_date( date );
    read_ssn( ssn );

    fix_name( name );
    fix_date( date );
    fix_ssn( ssn );

    individual_count++;

    if ( strlen( name ) > max_name_size )
      max_name_size = strlen( name );

    //bool date_cutoff = true;

    // for ( int i = 0; date[ i ] != '-'; i++ ) {
    //   if ( (int) AGE_CUTOFF[ i ] < (int) date[ i ] ) {
    //     date_cutoff = false;
    //     break;
    //   }
    // }

    if ( !( strcmp( AGE_CUTOFF, date ) < 0 ) ) {
      age_cutoff_count += 1;
    }
    
    if ( strcmp( ssn, "N/A" ) == 0 )
      no_ssn_count++;

    printf( "%-30s", name );
    printf( " " );
    printf( "%10s", date );
    printf( " " );
    printf( "%11s\n", ssn );

    // Reset each variable. This ensures that the old value is not retained if a new value is not included.
    strcpy( name, "" );
    strcpy( date, "" );
    strcpy( ssn, "" );
  }

  printf( "%c", '\n' );
  printf( "Summary\n" );
  printf( "-----------------------------------\n" );
  printf( "%-26s", "Total individuals" );
  printf( " " );
  printf( "%8d\n", individual_count );

  printf( "%-26s", "Maximum name length" );
  printf( " " );
  printf( "%8d\n", max_name_size );

  printf( "%-26s", "Individuals 21 or over" );
  printf( " " );
  printf( "%8d\n", age_cutoff_count );

  printf( "%-26s", "Individuals without an SS#" );
  printf( " " );
  printf( "%8d\n", no_ssn_count );

  return EXIT_SUCCESS;
}
