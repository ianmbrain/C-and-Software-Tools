#include <stdlib.h>
#include <stdio.h>
#include "account.h"
#include "transaction.h"

/** Index of the account file command-line argument. */
#define ACCOUNT_ARG 1

/** Index of the transacton file command-line argument. */
#define TRANSACT_ARG 2

int main( int argc, char *argv[] ) {
    loadAccounts( argv[ 1 ] );

    processTransactons( argv[ 2 ] );

    saveAccounts( argv[ 1 ] );
}
