/**
 * @file transaction.c
 * @author Ian M Brain (imbrain)
 * This file provides functionality to read in a transaction file.
 * The transactions are then processed and performed on the associated account balances.
 * This functionality is used in trader.c to process transactions on the account balances.
  */

#include "transaction.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"
#include "account.h"

/** Maximum of length of a transaction, in this case 'sell'. */
#define TRANS_LENGTH 4

/**
 * Processes the transactions from a transaction file and adjusts the associated account balances.
 * This functionality is used in trader.c to process transactions.
 * Throw file open error if the file cannot be opened
 * Throw invalid file error if the name does not exist as an account.
 * Throw invalid file error if the transaction is not buy or sell and if the shares or price cannot be read.
 * Throw account overflow error if the product of shares and price or transaction on the balance causes an overflow.
 * The code to read in price values is based on code from the loadAccounts() function in account.c.
 * @param fname name of the transaction file to process transactions from
*/
void processTransactons( char const fname[] ) {
    // Name of each account.
    char name[ NAME_LIMIT + 1 ] = "";
    // Buy or sell transaction.
    char transaction[ TRANS_LENGTH + 1 ];
    // Number of shares to be processed.
    unsigned long shares = 0;
    // Price of each share to be processed.
    char price[ 22 + 1 ] = "";

    FILE *file = fopen( fname, "r" );

    // Throw file open error if the file cannot be opened.
    if ( file == NULL ) {
        char print_error[ NAME_ERROR_LENGTH + AFILE_LIMIT + 1 ] = "Can't open transaction file: ";
        strcat( print_error, fname );
        fprintf( stderr, print_error );
    }

    while ( fscanf( file, "%s", name ) == 1 ) {
        // Throw invalid file error if a name in the transaction file does not match a name from the accont file.
        if ( lookupAccount( name ) == NULL ) {
            fprintf( stderr, "Invalid transaction file\n" );
            exit( EXIT_FAILURE );
        }

        // Throw invalid file error if the transaction is not buy or sell.
        fscanf( file, "%s", transaction );
        if ( strcmp( transaction, "buy" ) != 0 && strcmp( transaction, "sell" ) ) {
            fprintf( stderr, "Invalid transaction file\n" );
            exit( EXIT_FAILURE );
        }
        
        // Throw invalid file error if the shares cannot be read.
        if ( fscanf( file, "%ld", &shares ) != 1 ) {
            fprintf( stderr, "Invalid transaction file\n" );
            exit( EXIT_FAILURE );
        }

        // Read each price value.
        fscanf( file, "%s", price );

        // This code is based on code from the loadAccounts() function in account.c.
        // Current price character being read.
        char current_char = ' ';
        // Count of the decimals in the price string.
        int decimal_count = 0;
        // Count of digits after the decimal.
        int digits_after_decimal = 0;
        // Price as an integer to store the transformed string in.
        unsigned long int_price = 0;

        // Ensure the price is in the proper format and store it as an integer.
        for ( int i = 0; price[ i ]; i++ ) {
            current_char = price[ i ];

            if (current_char != '.' ) {
                if ( decimal_count > 0 ) {
                    digits_after_decimal++;

                    // Throw invalid file error if there are more than two digits after the decimal
                    if ( digits_after_decimal > DECIMAL_DIGITS ) {
                        fprintf( stderr, "Invalid transaction file" );
                        exit( EXIT_FAILURE );
                    }
                }

                // Convert the character number into an integer digit
                int current_digit = current_char - '0';

                // Throw account overflow error if adjusting the price value causes an overflow. Otherwise multiply by 10 to allow the return value to accomodate an additional digit.
                if ( checkMul( int_price, ADD_DIGIT ) == false ) {
                    fprintf( stderr, "Account overflow\n" );
                    exit( EXIT_FAILURE );
                }
                int_price *= ADD_DIGIT;

                // Throw account overflow error if adjusting the price value causes an overflow.
                if ( checkAdd( int_price, current_digit ) == false ) {
                    fprintf( stderr, "Account overflow\n" );
                    exit( EXIT_FAILURE );
                }
                int_price += current_digit;
            }
            else {
                decimal_count++;

                // Throw invalid file error if there are too many decimals.
                if ( decimal_count > 1 ) {
                    fprintf( stderr, "Invalid transaction file\n" );
                    exit( EXIT_FAILURE );
                }
            }
        }

        // Throw account overflow error if the product of shares and the price overflow.
        if ( !checkMul( shares, int_price ) ) {
            fprintf( stderr, "Account overflow\n" );
            exit( EXIT_FAILURE );
        }

        // Multiply  the shares by the price and store it in the account balance.
        unsigned long amount = shares * int_price;
        unsigned long *balance = lookupAccount( name );
        
        // Throw account overflow error if the transaction overflows the account balance.
        if ( strcmp( transaction, "buy" ) == 0 ) {
            if ( !checkSub( *balance, amount ) ) {
                fprintf( stderr, "Account overflow\n" );
                exit( EXIT_FAILURE );
            }
            *balance -= amount;
        }
        // Throw account overflow error if the transaction overflows the account balance.
        else {
            if ( !checkAdd( *balance, amount ) ) {
                fprintf( stderr, "Account overflow\n" );
                exit( EXIT_FAILURE );
            }
            *balance += amount;
        }
    }

    fclose( file );
}
