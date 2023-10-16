/**
 * @file account.c
 * @author Ian M Brain (imbrain)
 * This file provides functionality to load and save accounts from a file.
 * Other funtions are used to read in valid currency values and to return the balance of specific accounts.
 * This functionality is used in transaction.c to account balances as well as in trader.c to load and save the file.
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

/** Balances of each account. */
static unsigned long balances[ ACCOUNT_LIMIT ];

/** Names of each aacount, */
static char accounts[ ACCOUNT_LIMIT ][ NAME_LIMIT + 1 ];

/**
 * Reads in a currency value from the stream parameter and stores it in the pointer parameter.
 * The function returns true if the currency value is in the proper format and false otherwise.
 * Used in loadAccounts() to read the currency of each account in the file.
 * False is returned if the value overflows, it contains a character other than a digit or period, or there are too many digits after the decimal.
 * @param fp stream to read the currency value from.
 * @param val pointer to store the currency value in.
 * @return true if the currency can be correctly read and false otherwise.
*/
bool readCurrency( FILE *fp, unsigned long *val ) {
    // Count of how many decimals are in the currency value.
    int decimal_count = 0;
    // Count of how many digits exist in the currency value after the decimal.
    int digits_after_decimal = 0;
    // Currency values that will be returned from the string value.
    unsigned long return_value = 0;
    // String value of the currency value that will be converted into the return value.
    char string_value[ 21 + 10 ] = "";
    // Current character in the currency string value.
    char current_char = ' ';

    // Read the currency value as a string
    fscanf( fp, "%s", string_value );

    for ( int i = 0; string_value[ i ]; i++ ) {
        current_char = string_value[ i ];

        // Throw error if the value is not a digit or period.
        if ( current_char != '.' && !isdigit( current_char ) ) {
            return false;
        }

        if (current_char != '.' ) {
            if ( decimal_count > 0 ) {
                digits_after_decimal++;

                if ( digits_after_decimal > 2 ) {
                    return false;
                }
            }
            // Convert the character to an integer
            int current_digit = current_char - '0';

            // Throw and error if the value overflows.
            if ( checkMul( return_value, 10 ) == false ) {
                return false;
            }
            return_value *= 10;

            // Throw and error if the value overflows.
            if ( checkAdd( return_value, current_digit ) == false ) {
                return false;
            }
            return_value += current_digit;
        }
        else {
            decimal_count++;

            if ( decimal_count > 1 )
                return false;
        }
    }

    // If the value is valid, set the pointer to that value.
    *val = return_value;
    return true;
}

/**
 * Determines if the parameter name matches an existing account name.
 * Return a pointer to the account balance if there is a matching account name.
 * Return NULL if the name does not match any account name.
 * Used in processTransactions() to find the balance for each account to perform tranasctions on.
 * @param name name to check if there is a matching account name.
 * @return pointer to the balance of the account if the name matches and account and NULL otherwise.
*/
unsigned long *lookupAccount( char const name[ NAME_LIMIT + 1 ] ) {
    // Number of accounts that are being tracked.
    int number_of_accounts = sizeof( accounts ) / sizeof( accounts [ 0 ] );

    // Compare the name to each account name. Return the associated balance if the name matches.
    for ( int i = 0; i < number_of_accounts; i++ ) {
        if ( strcmp( name, accounts[ i ] ) == 0 ) {
            return balances + i;
        }
    }

    // Return null f the name is not the same as any account name.
    return NULL;
}

/**
 * Loads the accounts and balances from the parameter file.
 * Throw a file name error if the file name contains an invalid character or contains a value other than a digit after the dash.
 * Throw a file name error if any of the account names or balances have an invalid format.
 * Throw a file open error if the file cannot be opened.
 * @param fanme name of file to load the accounts from.
*/
void loadAccounts( char fname[ AFILE_LIMIT + 1 ] ) {
    // Count of dashes in the file name
    int dash_count = 0;
    // Error to print if the account name is incorrect
    char print_error[ 28 + AFILE_LIMIT + 1 ] = "Invalid account file name: ";
    strcat( print_error, fname );
    strcat( print_error, "\n" );

    // Read each character in the name. If the character is not a digit, character, dash or underscore, throw and error and exit the program.
    for ( int i = 0; fname[ i ]; i++ ) {
        if ( dash_count == 0 ) {
            if ( fname[ i ] == '-' )
                dash_count++;
            
            else if ( !isalpha( fname[ i ] ) && !isdigit( fname[ i ] ) && fname[ i ] != '_' ) {
                fprintf( stderr, print_error );
                exit( EXIT_FAILURE );
            }
        }
        // Throw and error if there are any non digit characters besides the file type after the dash.
        else {
            if( !isdigit( fname[ i ] ) ) {
                // Break for periods indicating the file type.
                if ( fname[ i ] == '.' )
                    break;

                fprintf( stderr, print_error );
                exit( EXIT_FAILURE );
            }
        }
    }

    // Open the stream associated with the file name.
    FILE *file = fopen( fname, "r" );

    // Throw an error if the file cannot be opened.
    if ( file == NULL ) {
        char print_error[ 25 + AFILE_LIMIT + 1 ] = "Can't open account file: ";
        strcat( print_error, fname );
        strcat( print_error, "\n" );
        fprintf( stderr, print_error );
        exit( EXIT_FAILURE );
    }

    // Index tracker for the account and balance
    int current_acc = 0;
    // Name of each account in the file. 
    char account_name[ NAME_LIMIT + 1 ];

    while ( fscanf( file, "%s", account_name ) == 1 ) {
        // Copy the name into accounts. Throw an error if the account name is too long.
        if ( account_name[ NAME_LIMIT ] != '\0' ) {
            fprintf( stderr, "Invalid account file\n" );
            exit( EXIT_FAILURE );
        }
        strcpy( accounts[ current_acc ], account_name );

        // Read the currency and store it as a balance. Throw an error if the currency cannot be read.
        if ( readCurrency( file, ( balances + current_acc ) ) == false ) {
            fprintf( stderr, "Invalid account file\n" );
            exit( EXIT_FAILURE );
        }

        current_acc++;
    }

    fclose( file );
}

/**
 * Writes the account names and balances as doubles to the parameter file.
 * File number is adjusted by one before it is written to.
 * Throw file name error if the file name becomes too long after adjusting the file number.
 * Throw file open error if the file cannot be opened.
 * @param fname name of file to save the accounts and balances to.
*/
void saveAccounts( char fname[ AFILE_LIMIT + 1 ] ) {
    // Number of the file that will be adjusted by one.
    char file_num[ AFILE_LIMIT + 1 ] = "";
    // Index of the dash in the file name.
    int dash_index = 0;
    // Index used for parsing the file number.
    int num_index = 0;

    // Separate the file number and store it in the number variable.
    for ( int i = 0; fname[ i ]; i++ ) {
        if ( dash_index > 0 ) {
            // Stop counting the file number at the period which indicates the file type.
            if ( fname[ i ] == '.' )
                break;

            file_num[ num_index ] = fname[ i ];
            num_index++;
        }
        else if ( fname[ i ] == '-' )
            dash_index = i;
    }

    // String to store the adjusted file name
    char new_fname[ AFILE_LIMIT + 1 ] = " ";
    // File number adjusted by one.
    int new_num = atoi( file_num );
    new_num++;
    sprintf( file_num, "%d", new_num );

    // Copy the file name up to the dash to the new file name. Add the adjusted file number and file type.
    strncpy( new_fname, fname, dash_index + 1 );
    strcat( new_fname, file_num );
    strcat( new_fname, ".txt" );

    // Error message to throw if the account name is invalid
    char print_error[ 28 + AFILE_LIMIT + 1 ] = "Invalid account file name: ";
    strcat( print_error, fname );
    strcat( print_error, "\n" );

    // Throw a file name error if the last character is not a new line character idicating the name is too long.
    if ( new_fname[ strlen( new_fname ) + 1 ] != '\0' ) {
        fprintf( stderr, print_error );
        exit( EXIT_FAILURE );
    }

    FILE *file = fopen( new_fname, "w" );

    // Throw a file open error if the file cannot be opened.
    if ( file == NULL ) {
        char print_error[ 25 + AFILE_LIMIT + 1 ] = "Can't open account file: ";
        strcat( print_error, fname );
        strcat( print_error, "\n" );
        fprintf( stderr, print_error );
        exit( EXIT_FAILURE );
    }

    // Index of the current account and balance.
    int current_acc = 0;
    // Length of the file.
    int file_length = 0;

    // Determine the length of accounts by counting the accounts until they are empty.
    for ( int i = 0; strcmp( accounts[ i ], "" ) != 0; i++ ) {
        file_length++;
    }

    // Balance of each account.
    long double acc_balance = 0;

    // Print the file name and double balance to the file
    while ( current_acc < file_length ) {
        fprintf( file, "%-30s", accounts[ current_acc ] );

        // Convert each integer balance into a double.
        acc_balance = ( long double ) balances[ current_acc ] / 100;
        fprintf( file, "%22.2Lf\n", acc_balance );

        current_acc++;
    }

    fclose( file );
}
