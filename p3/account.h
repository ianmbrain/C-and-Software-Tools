/**
 * @file account.h
 * @author Ian M Brain (imbrain)
 * This file provides functionality to load and save accounts from a file.
 * Other funtions are used to read in valid currency values and to return the balance of specific accounts.
 * This functionality is used in transaction.c to account balances as well as in trader.c to load and save the file.
  */


#include <stdbool.h>
#include <stdio.h>

/** Maximum length of an account name. */
#define NAME_LIMIT 30

/** Limit on the length of an account file. */
#define AFILE_LIMIT 30

/** Number of digits permitted to be after the decimal point. */
#define DECIMAL_DIGITS 2

/** Value to multiply the return value value by to allow it to permit an additional digit. */
#define ADD_DIGIT 10

/** Length of the invalid name error text. */
#define NAME_ERROR_LENGTH 27

/** Length of the ULONG value to be used to intitalize strings that hold these values. */
#define ULONG_LENGTH 20

/**
 * Reads in a currency value from the stream parameter and stores it in the pointer parameter.
 * The function returns true if the currency value is in the proper format and false otherwise.
 * Used in loadAccounts() to read the currency of each account in the file.
 * False is returned if the value overflows, it contains a character other than a digit or period, or there are too many digits after the decimal.
 * @param fp stream to read the currency value from.
 * @param val pointer to store the currency value in.
 * @return true if the currency can be correctly read and false otherwise.
*/
bool readCurrency( FILE *fp, unsigned long *val );

/**
 * Determines if the parameter name matches an existing account name.
 * Return a pointer to the account balance if there is a matching account name.
 * Return NULL if the name does not match any account name.
 * Used in processTransactions() to find the balance for each account to perform tranasctions on.
 * @param name name to check if there is a matching account name.
 * @return pointer to the balance of the account if the name matches and account and NULL otherwise.
*/
unsigned long *lookupAccount( char const name[ NAME_LIMIT + 1 ] );

/**
 * Loads the accounts and balances from the parameter file.
 * Throw a file name error if the file name contains an invalid character or contains a value other than a digit after the dash.
 * Throw a file name error if any of the account names or balances have an invalid format.
 * Throw a file open error if the file cannot be opened.
 * @param fanme name of file to load the accounts from.
*/
void loadAccounts( char fname[ AFILE_LIMIT + 1 ] );

/**
 * Writes the account names and balances as doubles to the parameter file.
 * File number is adjusted by one before it is written to.
 * Throw file name error if the file name becomes too long after adjusting the file number.
 * Throw file open error if the file cannot be opened.
 * @param fname name of file to save the accounts and balances to.
*/
void saveAccounts( char fname[ AFILE_LIMIT + 1 ] );
