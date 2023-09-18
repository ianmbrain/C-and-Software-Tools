/**
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

// Seems like fields are separated by ':' and individuals are separated by '\n'
//The line should start with the individual’s name in standard format, printed left justified in a 30-character field. 
// This should be followed by a space and then the individual’s birthdate in standard format (ten characters). 
// After another space, the individual’s SSN should be printed in standard format, right justified in an 11-character field. The SSN will take all 11 characters, unless it’s N/A.

int main() {
  int individual_count = 0;
  int max_name_size = 0;
  int age_cutoff_count = 0;
  int no_ssn_count = 0;

  char name[ FIELD_MAX + 1 ];
  char date[ FIELD_MAX + 1 ];
  char ssn[ FIELD_MAX + 1 ];

  while ( read_name( name ) != false ) {
    read_date( date );
    read_ssn( ssn );

    individual_count++;

    if ( strlen( name ) > max_name_size )
      max_name_size = strlen( name );

    bool meet_cutoff = true;
    for ( int i = 0; date[ i ]; i++ ) {
      if ( AGE_CUTOFF[ i ] > date[ i ] )
        meet_cutoff = false;
    }

    if ( meet_cutoff ) {
      age_cutoff_count++;
    }
    
    if ( strcmp( ssn, "N/A" ) == 0 )
      no_ssn_count++;

    printf( "%-30s", name );
    printf( " " );
    printf( "%10s", date );
    printf( " " );
    printf( "%11s\n", ssn );
  }

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
