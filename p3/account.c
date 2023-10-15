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

static unsigned long balances[ 100000 ];

static char accounts[ 100000 ][ NAME_LIMIT + 1 ];

// This function reads an amount of currency from the given stream, storing it in the unsigned value pointed to by val. 
// It returns true if the input contains an amount of currency in a valid format for an account 
// or transaction file and can be represented as cents in an unsigned long.

// May return false if the value overflows indicating a failure to read the value
// non negative value NO ONLY FOR BEGINNING BALANCES
// may need to check if the first value is a decimal when it should be '0' or something
bool readCurrency( FILE *fp, unsigned long *val ) {
    int decimal_count = 0;
    int digits_after_decimal = 0;
    int decimal_digit = 0;

    unsigned long return_value = 0;
    char string_value[ 21 + 10 ] = "";

    fscanf( fp, "%s", string_value );
    //fprintf( stderr,  "%s\n", string_value );

    // read this in. read each character from it

    char current_char = ' ';
    
    //int value_length = strlen( string_value );

    for ( int i = 0; string_value[ i ]; i++ ) {
        current_char = string_value[ i ];
        //fprintf( stderr, "%c\n", current_char );

        if (current_char != '.' ) {
            if ( decimal_count > 0 ) {
                digits_after_decimal++;

                if ( digits_after_decimal > 2 ) {
                    return false;
                }
            }
            int current_digit = current_char - '0';

            checkMul( return_value, 10 );
            return_value *= 10;

            checkAdd( return_value, current_digit );
            return_value += current_digit;

            // if ( decimal_count == 0 ) {
            //     checkMul( return_value, 10 );
            //     return_value *= 10;

            //     checkAdd( return_value, current_digit );
            //     return_value += current_digit;
            // }
            // else {
            //     decimal_digit++;
                
            //     if ( decimal_digit == 1 ) {
            //         if ( !( current_digit <= 1 ) )
            //             return false;

            //         current_digit /= 10;
            //         return_value += current_digit;
            //     }
            //     else if ( decimal_digit == 2 ) {
            //         if ( current_digit > 5 )
            //             return false;

            //         current_digit /= 100;
            //         return_value += current_digit;
            //     }
            //     else if (decimal_digit > 2 )
            //         return false;
            // }
        }
        else {
            decimal_count++;

            if ( decimal_count > 1 )
                return false;
        }
    }
//_______________________________________________________________

    // char current_char = fgetc( fp );

    // while ( isspace( current_char ) ) {
    //     current_char = fgetc( fp );
    // }
    
    // // Read in the value
    // while ( isdigit( current_char ) || current_char == '.' ) {
    //     // ATOI may cause problems with garbage ________---_____--___--__-__-_--_-_-_-_-------______------_---_--_-
    //     fprintf( stderr, "%c\n", current_char );

    //     if (current_char != '.' ) {
    //         int current_digit = current_char - 0;

    //         if ( decimal_count == 0 ) {
    //             checkMul( return_value, 10 );
    //             return_value *= 10;

    //             checkAdd( return_value, current_digit );
    //             return_value += current_digit;
    //         }
    //         else {
    //             decimal_digit++;
                
    //             if ( decimal_digit == 1 ) {
    //                 if ( !( current_digit <= 1 ) )
    //                     return false;

    //                 current_digit /= 10;
    //                 return_value += current_digit;
    //             }
    //             else if ( decimal_digit == 2 ) {
    //                 if ( current_digit > 5 )
    //                     return false;

    //                 current_digit /= 100;
    //                 return_value += current_digit;
    //             }
    //             else if (decimal_digit > 2 )
    //                 return false;
    //         }
    //     }
    //     else {
    //         decimal_count++;

    //         if ( decimal_count > 1 )
    //             return false;
    //     }

    //     //fprintf( stderr, "%ld\n", return_value );
        
    //     current_char = fgetc( fp );
    // }

    *val = return_value;
    return true;

// Check for the correct format - two last digits then decimal
}

// The account.c component maintains a list of all account names and a list of balances. 
// This returns a pointer to the balance value for an account with the given name. 
// If no such account exits, it returns NULL. This function can be used by the 
// transaction to get the balance value for an account and adjust it based on the current transaction.
unsigned long *lookupAccount( char const name[ NAME_LIMIT + 1 ] ) {
    for ( int i = 0; name[ i ]; i++ ) {
        if ( strcmp( name, accounts[ i ] ) == 0 ) {
            return balances + i;
        }
    }

    return NULL;
}

// This function loads all the accounts from the file with the given account name, detecting any errors in the filename or the file contents.
void loadAccounts( char fname[ AFILE_LIMIT + 1 ] ) {
    // STILL NEED TO check for errors in the account name
    // A name 
    // This should be from that file with the fname

    //If part of the account file is incorrect (e.g., too many accounts, 
    // an account name that’s too long or a bad character in an account balance), the program should report the following error message.

    int dash_count = 0;
    char print_error[ 28 + AFILE_LIMIT + 1 ] = "Invalid account file name: ";
    strcat( print_error, fname );

    for ( int i = 0; fname[ i ]; i++ ) {
        if ( dash_count == 0 ) {
            if ( fname[ i ] == '-' )
                dash_count++;
            
            else if ( !isalpha( fname[ i ] ) && !isdigit( fname[ i ] ) && fname[ i ] != '_' ) {
                fprintf( stderr, print_error );
                exit( EXIT_FAILURE );
            }
        }
        else {
            if( !isdigit( fname[ i ] ) ) {
                // If the file is the . for the file type, the file name is successful
                if ( fname[ i ] == '.' )
                    break;

                fprintf( stderr, print_error );
                exit( EXIT_FAILURE );
            }
        }
    }

    // May not be needed
    FILE *file = fopen( fname, "r" );

    if ( file == NULL ) {
        char print_error[ 25 + AFILE_LIMIT + 1 ] = "Can't open account file: ";
        strcat( print_error, fname );
        fprintf( stderr, print_error );
        exit( EXIT_FAILURE );
    }

    int current_acc = 0;
    //char acc_name[ NAME_LIMIT + 1 ];
    //char acc_balance[ 20 ] = " ";

    while ( fscanf( file, "%s", accounts[ current_acc ] ) == 1 ) {
        // Check for correct account name format

        // Read currency 
        readCurrency( file, ( balances + current_acc ) );


        current_acc++;
    }

    fprintf( stderr, "%d", current_acc );
    fclose( file );
}

// After processing all input transactions, this function can be used to write out the updated balances of all accounts. 
// It writes to the next version of the given account file name (i.e., the given name with the version number stepped).
void saveAccounts( char fname[ AFILE_LIMIT + 1 ] ) {
    //char file_name[ AFILE_LIMIT + 1 ] = " ";
    char file_num[ AFILE_LIMIT + 1 ] = " ";
    int dash_index = 0;
    int num_index = 0;
    
    for ( int i = 0; fname[ i ]; i++ ) {
        if ( dash_index > 0 ) {
            // If the file is the . for the file type, stop counting the file number
            if ( fname[ i ] == '.' )
                break;

            file_num[ num_index ] = fname[ i ];
            num_index++;
        }
        else if ( fname[ i ] == '-' )
            dash_index = i;
    }

    //fprintf( stderr, file_num );

    char new_fname[ AFILE_LIMIT + 1 ] = " ";
    int new_num = atoi( file_num );
    new_num++;
    sprintf( file_num, "%d", new_num );

    strncpy( new_fname, fname, dash_index + 1 );
    strcat( new_fname, file_num );
    strcat( new_fname, ".txt" );

    // Check for overflow
    //int dash_count = 0;
    char print_error[ 28 + AFILE_LIMIT + 1 ] = "Invalid account file name: ";
    strcat( print_error, fname );

    if ( new_fname[ strlen( new_fname ) + 1 ] != '\0' && !isspace( new_fname[ strlen( new_fname ) + 1 ] ) ) {
        fprintf( stderr, print_error );
        exit( EXIT_FAILURE );
    }

    FILE *file = fopen( new_fname, "w" );
    if ( file == NULL ) {
        char print_error[ 25 + AFILE_LIMIT + 1 ] = "Can't open account file: ";
        strcat( print_error, fname );
        fprintf( stderr, print_error );
        exit( EXIT_FAILURE );
    }

    int current_acc = 0;
    int file_length = 0; //( sizeof( balances ) / sizeof( balances[ 0 ] ) );
    for ( int i = 0; strcmp( accounts[ i ], "" ) != 0; i++ ) {
        file_length++;
    }

    while ( current_acc < file_length ) {
        fprintf( file, "%-30s", accounts[ current_acc ] );
        fprintf( file, "%ld\n", balances[ current_acc ] );

        current_acc++;
    }

    fclose( file );
}
