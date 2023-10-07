/**
 */

#include "account.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"

/** Number of accounts supported by the program. */
#define ACCOUNT_LIMIT 100000

// This function reads an amount of currency from the given stream, storing it in the unsigned value pointed to by val. 
// It returns true if the input contains an amount of currency in a valid format for an account 
// or transaction file and can be represented as cents in an unsigned long.

// May return false if the value overflows indicating a failure to read the value
// non negative value NO ONLY FOR BEGINNING BALANCES
// may need to check if the first value is a decimal when it should be '0' or something
bool readCurrency( FILE *fp, unsigned long *val ) {
    int decimal_count = 0;
    int decimal_digit = 0;

    unsigned long return_value = 0;
    char current_char = fgetc( fp );
  
    // Read in the value
    while ( isdigit( current_char ) || current_char == '.' ) {
        // ATOI may cause problems with garbage ________---_____--___--__-__-_--_-_-_-_-------______------_---_--_-
        if (current_char != '.' ) {
            int current_digit = atoi( current_char );

            if ( decimal_count == 0 ) {
                checkMul( return_value, 10 );
                return_value *= 10;

                checkAdd( return_value, current_digit );
                return_value += current_digit;
            }
            else {
                decimal_digit++;
                
                if ( decimal_digit == 1 ) {
                    if ( current_digit != 1 )
                        return false;

                    current_digit /= 10;
                    return_value += current_digit;
                }
                else if ( decimal_digit == 1 ) {
                    if ( current_digit > 5 )
                        return false;

                    current_digit /= 100;
                    return_value += current_digit;
                }
                else if (decimal_digit > 2 )
                    return false;
            }
        }
        else {
            decimal_count++;

            if ( decimal_count > 1 )
                return false;
        }
        
        current_char = fgetc( fp );
    }

// Check for the correct format - two last digits then decimal
}

// The account.c component maintains a list of all account names and a list of balances. 
// This returns a pointer to the balance value for an account with the given name. 
// If no such account exits, it returns NULL. This function can be used by the 
// transaction to get the balance value for an account and adjust it based on the current transaction.
unsigned long *lookupAccount( char const name[ NAME_LIMIT + 1 ] ) {

}

//
void loadAccounts( char fname[ AFILE_LIMIT + 1 ] ) {

}

void saveAccounts( char fname[ AFILE_LIMIT + 1 ] ) {

}
