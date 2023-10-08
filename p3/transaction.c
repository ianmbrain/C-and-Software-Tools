/**
 */

// This function reads and performs all transactions in the transaction file with the given name. 
// It uses the account component to update account balances and detects errors as the transaction file is processed.
void processTransactons( char const fname[] ) {
    // If there is an overflow during a transaction, either in computing the product of the number of shares and the share price
    // or in increasing or decreasing the account balance

    //If there is some other problem with the contents of the transaction file (e.g., an account name that isn’t in the account file, 
    // a transaction type that isn’t “buy” or “sell” or a number of shares that can’t be parsed as an unsigned long)

    // An invalid currency value or a currency value that’s too large should also produce the Invalid transaction file message. You are not expected to check for overflow in reading the number of shares

    
}
