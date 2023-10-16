/**
 */

// I included these, check what libraries we can use __________--_____----_____-------_____----______-------_____----_____------------
#include "transaction.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"
#include "account.h"

// This function reads and performs all transactions in the transaction file with the given name. 
// It uses the account component to update account balances and detects errors as the transaction file is processed.
void processTransactons( char const fname[] ) {
    // If there is an overflow during a transaction, either in computing the product of the number of shares and the share price
    // or in increasing or decreasing the account balance

    //If there is some other problem with the contents of the transaction file (e.g., an account name that isn’t in the account file, 
    // a transaction type that isn’t “buy” or “sell” or a number of shares that can’t be parsed as an unsigned long)

    // An invalid currency value or a currency value that’s too large should also produce the Invalid transaction file message. You are not expected to check for overflow in reading the number of shares

    char name[ NAME_LIMIT + 1 ] = "";
    char transaction[ 4 + 1 ];
    unsigned long shares = 0;
    char price[ 22 + 1 ] = "";

    FILE *file = fopen( fname, "r" );
    if ( file == NULL ) {
        char print_error[ 29 + AFILE_LIMIT + 1 ] = "Can't open transaction file: ";
        strcat( print_error, fname );
        fprintf( stderr, print_error );
    }

    while ( fscanf( file, "%s", name ) == 1 ) {
        if ( lookupAccount( name ) == NULL ) {
            fprintf( stderr, "Invalid transaction file\n" );
            exit( EXIT_FAILURE );
        }
        
        fscanf( file, "%s", transaction );
        if ( strcmp( transaction, "buy" ) != 0 && strcmp( transaction, "sell" ) ) {
            fprintf( stderr, "Invalid transaction file\n" );
            exit( EXIT_FAILURE );
        }
        
        if ( fscanf( file, "%ld", &shares ) != 1 ) {
            fprintf( stderr, "Invalid transaction file\n" );
            exit( EXIT_FAILURE );
        }

        // Read in each price tranforming it into an int.
        fscanf( file, "%s", price );

        char current_char = ' ';
        int decimal_count = 0;
        int digits_after_decimal = 0;
        unsigned long int_price = 0;

        // Convert price to an integer.
        for ( int i = 0; price[ i ]; i++ ) {
            current_char = price[ i ];

            if (current_char != '.' ) {
                if ( decimal_count > 0 ) {
                    digits_after_decimal++;

                    if ( digits_after_decimal > 2 ) {
                        fprintf( stderr, "Invalid transaction file" );
                        exit( EXIT_FAILURE );
                    }
                }
                unsigned long current_digit = current_char - '0';
                //fprintf( stderr, "%ld\n", current_digit );

                if ( checkMul( int_price, 10 ) == false ) {
                    fprintf( stderr, "Account overflow\n" );
                    exit( EXIT_FAILURE );
                }
                int_price *= 10;
                // fprintf( stderr, "%ld - ", sizeof(10) );
                // fprintf( stderr, "%ld - ", int_price );
                // fprintf( stderr, "%ld - ", current_digit );

                if ( checkAdd( int_price, current_digit ) == false ) {
                    fprintf( stderr, "Account overflow\n" );
                    exit( EXIT_FAILURE );
                }
                int_price += current_digit;
                //fprintf( stderr, "%ld\n", int_price );
            }
            else {
                decimal_count++;

                if ( decimal_count > 1 ) {
                    fprintf( stderr, "Invalid transaction file\n" );
                    exit( EXIT_FAILURE );
                }
            }
        }

        if ( !checkMul( shares, int_price ) ) {
            fprintf( stderr, "Account overflow\n" );
            exit( EXIT_FAILURE );
        }

        unsigned long amount = shares * int_price;
        unsigned long *balance = lookupAccount( name );
        
        if ( strcmp( transaction, "buy" ) == 0 ) {
            if ( !checkSub( *balance, amount ) ) {
                fprintf( stderr, "Account overflow\n" );
                exit( EXIT_FAILURE );
            }

            *balance -= amount;
        }
        else {
            if ( !checkAdd( *balance, amount ) ) {
                fprintf( stderr, "Account overflow\n" );
                exit( EXIT_FAILURE );
            }

            *balance += amount;
        }

        // fprintf( stderr, "%s ", name );
        // fprintf( stderr, "%ld\n", *balance );
    }

    //saveAccounts( fname );

    fclose( file );
}
