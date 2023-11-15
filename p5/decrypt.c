
//This is the main component for the decrypt program. 
//It contains the main function and uses the other components to read the input file, 
//to perform decryption and to write out the plaintext output.

#include "io.h"
#include "DES.h"

#define CMD_ARGS 3

/** Command line argument that contains the decryption key. */
#define CMD_KEY 0

/** Command line argument that contains the file name. */
#define CMD_FILE 1

int main( int argc, char *argv[] ) {
    if ( argc != CMD_ARGS ) {
        fprintf( stderr, "usage: decrypt <key-file> <input-file> <output-file>\n");
        exit( EXIT_FAILURE );
    }

    // Open the file to encrypt.
    FILE *file = fopen( argv[ CMD_FILE ], "rb" );

    // If the file cannot be decrypted exit as failure.
    if ( file == NULL ) {
        perror( argv[ CMD_FILE ] );
        exit( EXIT_FAILURE );
    }

    // If the decryption key is longer than allowed exit as failure.
    if ( strlen( argv[ CMD_KEY ] ) > 8 ) {
        fprintf( stderr, "Key too long\n");
        exit( EXIT_FAILURE );
    }

    // If the decryption key length is not a multiple of eight exit as failure.
    if ( strlen( argv[ CMD_FILE ] ) % 8 != 0 ) {
        fprintf( stderr, "%s%s\n" "Bad ciphertext file length: ", argv[ CMD_FILE ] );
        exit( EXIT_FAILURE );
    }

    DESBlock *block = ( DESBlock * )malloc( sizeof( DESBlock ) );
    block->len = 8;
}
