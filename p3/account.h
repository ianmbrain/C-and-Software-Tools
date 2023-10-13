/**
 */

#include <stdbool.h>
#include <stdio.h>

/** Maximum length of an account name. */
#define NAME_LIMIT 30

/** Limit on the length of an account file. */
#define AFILE_LIMIT 30

bool readCurrency( FILE *fp, unsigned long *val );

unsigned long *lookupAccount( char const name[ NAME_LIMIT + 1 ] );

void loadAccounts( char fname[ AFILE_LIMIT + 1 ] );

void saveAccounts( char fname[ AFILE_LIMIT + 1 ] );
