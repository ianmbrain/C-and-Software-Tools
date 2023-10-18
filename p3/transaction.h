/**
 * @file transaction.h
 * @author Ian M Brain (imbrain)
 * This file provides functionality to read in a transaction file.
 * The transactions are then processed and performed on the associated account balances.
 * This functionality is used in trader.c to process transactions on the account balances.
  */

/**
 * Processes the transactions from a transaction file and adjusts the associated account balances.
 * This functionality is used in trader.c to process transactions.
 * Throw file open error if the file cannot be opened
 * Throw invalid file error if the name does not exist as an account.
 * Throw invalid file error if the transaction is not buy or sell and if the shares or price cannot be read.
 * Throw account overflow error if the product of shares and price or transaction on the balance causes an overflow.
 * The code to read in price values is based on code from the loadAccounts() function in account.c.
 * @param fname name of the transaction file to process transactions from.
*/
void processTransactons( char const fname[] );
