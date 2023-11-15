
//This is the main component for the encrypt program. 
//It contains the main function and uses the other components to read the input file, 
//to perform encryption and to write out the ciphertext output.

#include "io.h"
#include "DES.h" // ___--_____---_____--____--_---_--_--__--_----
#include <string.h>

/** Expected number of command line arguments. */
#define CMD_ARGS 3

/** Command line argument that contains the encryption key. */
#define CMD_KEY 0

/** Command line argument that contains the file name. */
#define CMD_FILE 1

/** Command line argument that contains the output file name. */
#define CMD_OUTPUT 2


int main( int argc, char *argv[] ) {
    if ( argc != CMD_ARGS ) {
        fprintf( stderr, "usage: encryt <key> <input_file> <output_file>\n");
        exit( EXIT_FAILURE );
    }

    // Open the file to encrypt.
    FILE *file = fopen( argv[ CMD_FILE ], "rb" );

    // If the file cannot be encrypted exit as failure.
    if ( file == NULL ) {
        perror( argv[ CMD_FILE ] );
        exit( EXIT_FAILURE );
    }

    // If the encryption key is longer than allowed exit as failure.
    if ( strlen( argv[ CMD_KEY ] ) > 8 ) {
        fprintf( stderr, "Key too long\n");
        exit( EXIT_FAILURE );
    }

    DESBlock *block = ( DESBlock * )malloc( sizeof( DESBlock ) );

    readBlock( file, block );

    // Convert the provided key into bytes.
    byte key[ BLOCK_BYTES ] = {};
    prepareKey( key, argv[ CMD_KEY ] );

    // Generate the subkeys from the provided key.
    byte K[ ROUND_COUNT ][ SUBKEY_BYTES ] = {};
    generateSubkeys( K, key );

    // Encrypt the file.
    encryptBlock( block, K );

    FILE *ouput = fopen( argv[ CMD_OUTPUT ], "rb" );
}
