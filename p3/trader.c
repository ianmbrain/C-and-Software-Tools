/**
 * @file trader.c
 * @author Ian M Brain (imbrain)
 * This program provides funtionality to read in accounts and perform transactions on those accounts.
 * Accounts have a name and a balance.
 * Transactions have a name, transaction type, shares and price.
 * Transactions are performed on the accounts and the resulting accounts and balances are saved to a new file with its file number adjusted by one.
  */

#include <stdlib.h>
#include <stdio.h>
#include "account.h"
#include "transaction.h"

/** Index of the account file command-line argument. */
#define ACCOUNT_ARG 1

/** Index of the transacton file command-line argument. */
#define TRANSACT_ARG 2

/**
 * Throw usage error if there are too many command line arguments.
 * Load in the account file to store the accounts and balances.
 * Process transactions and adjust the associated account balances.
 * Save the accounts and balances to a new file with its file number adjusted by one.
 * @param argc number of command line arguments.
 * @param argv array of command line arguments.
 * @return return value of sucess or failure.
*/
int main( int argc, char *argv[] ) {
    // Throw error if there are too many command line arguments.
    if ( argc >  TRANSACT_ARG + ACCOUNT_ARG ) {
        fprintf( stderr, "usage: trader <account-file> <transaction-file>\n" );
        exit( EXIT_FAILURE );
    }
    loadAccounts( argv[ ACCOUNT_ARG ] );

    processTransactons( argv[ TRANSACT_ARG ] );

    saveAccounts( argv[ ACCOUNT_ARG ] );
}
