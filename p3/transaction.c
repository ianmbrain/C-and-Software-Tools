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

    char name[ NAME_LIMIT + 1 ];
    char transaction[ 4 + 1 ];
    unsigned long shares = 0;
    double price = 0;

    FILE *file = fopen( fname, "r" );
    if ( file == NULL ) {
        char print_error[ 29 + AFILE_LIMIT + 1 ] = "Can't open transaction file: ";
        strcat( print_error, fname );
        fprintf( stderr, print_error );
    }

    while ( fscanf( file, "%s%s%ld%lf", name, transaction, &shares, &price ) == 1 ) {
        if ( lookupAccount( name ) == NULL ) {
            fprintf( stderr, "Invalid transaction file" );
            exit( EXIT_FAILURE );
        }
        
        fscanf( file, "%s", transaction );
        if ( strcmp( transaction, "buy" ) != 0 && strcmp( transaction, "sell" ) ) {
            fprintf( stderr, "Invalid transaction file" );
            exit( EXIT_FAILURE );
        }
        
        if ( fscanf( file, "%ld", &shares ) != 1 ) {
            fprintf( stderr, "Invalid transaction file" );
            exit( EXIT_FAILURE );
        }

        fscanf( file, "%lf", &price );

        if ( !checkMul( shares, price ) ) {
            fprintf( stderr, "Account overflow" );
            exit( EXIT_FAILURE );
        }

        unsigned long amount = shares * price;
        unsigned long *balance = lookupAccount( name );
        if ( !checkAdd( *balance, amount ) ) {
            fprintf( stderr, "Account overflow" );
            exit( EXIT_FAILURE );
        }
        
        *balance += amount;
    }

    //saveAccounts( fname );

    fclose( file );
}
