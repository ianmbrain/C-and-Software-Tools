
//This is the main component for the encrypt program. 
//It contains the main function and uses the other components to read the input file, 
//to perform encryption and to write out the ciphertext output.

#include "io.h"
#include "DES.h" // ___--_____---_____--____--_---_--_--__--_----
#include <string.h>

/** Expected number of command line arguments. */
#define CMD_ARGS 4

/** Command line argument that contains the encryption key. */
#define CMD_KEY 1

/** Command line argument that contains the file name. */
#define CMD_FILE 2

/** Command line argument that contains the output file name. */
#define CMD_OUTPUT 3

void printBits( byte const data[], int bcount )
{
    for ( int i = 1; i <= bcount; i++ ) {
        fprintf( stderr, "\n%c\n", getBit( data, i ) ? '1' : '0' );
        if ( i % 8 == 0 ) {
            fprintf( stderr, "\n%d ----------- \n", i / 8 );
        }
    }
}

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
    // printBits( block->data, (8*8) );

    // Convert the provided key into bytes.
    byte key[ BLOCK_BYTES ] = {};
    prepareKey( key, argv[ CMD_KEY ] );
    // printBits( key, (8*8) );

    // Generate the subkeys from the provided key.
    byte K[ ROUND_COUNT ][ SUBKEY_BYTES ] = {};
    generateSubkeys( K, key );
    // printBits( K[ 16 ], (6 * 8) );

    // Encrypt the file.
    encryptBlock( block, K );
    // printBits( block->data, (8*8) );

    // Write the encrypted file to the provided output file.
    FILE *output = fopen( argv[ CMD_OUTPUT ], "wb" );
    writeBlock( output, block );
    // printBits( block->data, (8*8) );

    // FILE *testout = fopen( "cipher-a.bin", "rb" );
    // DESBlock *blockout = ( DESBlock * )malloc( sizeof( DESBlock ) );
    // readBlock( testout, blockout );
    // printBits( blockout->data, (8*8) );
    // fclose (testout);

    // printBits( block->data, 10 );
}
