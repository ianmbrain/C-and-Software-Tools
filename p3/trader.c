#include <stdlib.h>
#include <stdio.h>
#include "account.h"
#include "transaction.h"

/** Index of the account file command-line argument. */
#define ACCOUNT_ARG 1

/** Index of the transacton file command-line argument. */
#define TRANSACT_ARG 2

int main( int argc, char *argv[] ) {
    // Throw error if there are too many arguments.
    if ( argc > 3 ) {
        fprintf( stderr, "usage: trader <account-file> <transaction-file>\n" );
        exit( EXIT_FAILURE );
    }
    loadAccounts( argv[ 1 ] );

    processTransactons( argv[ 2 ] );

    saveAccounts( argv[ 1 ] );
}
