
//This is the main component for the decrypt program. 
//It contains the main function and uses the other components to read the input file, 
//to perform decryption and to write out the plaintext output.

#include "io.h"
#include "DES.h"
#include <string.h>

/** Expected number of command line arguments. */
#define CMD_ARGS 4

/** Command line argument that contains the decryption key. */
#define CMD_KEY 1

/** Command line argument that contains the file name. */
#define CMD_FILE 2

/** Command line argument that contains the output file name. */
#define CMD_OUTPUT 3

int main( int argc, char *argv[] ) {
    // Exit as failure if there are an invalid number of command arguments.
    if ( argc != CMD_ARGS ) {
        fprintf( stderr, "usage: decrypt <key> <input_file> <output_file>\n");
        exit( EXIT_FAILURE );
    }

    // Open the input file.
    FILE *file = fopen( argv[ CMD_FILE ], "rb" );

    // Exit as failure if the input file cannot be opened.
    if ( file == NULL ) {
        perror( argv[ CMD_FILE ] );
        exit( EXIT_FAILURE );
    }

    // ALREADY INCLUDED IN PREPARE KEY
    // Exit as failure if the encryption key is longer than allowed.
    // if ( strlen( argv[ CMD_KEY ] ) > 8 ) {
    //     fprintf( stderr, "Key too long\n");
    //     exit( EXIT_FAILURE );
    // }

    // Determine the length of the input file.
    // Length of the file.
    int file_length = 0;
    fseek( file, 0, SEEK_END );
    file_length = ftell( file );
    fseek( file, -file_length, SEEK_END );

    // Exit as failure if the decryption file length is not a multiple of eight.    
    if ( file_length % 8 != 0 ) {
        fprintf( stderr, "%s%s\n", "Bad ciphertext file length: ", argv[ CMD_FILE ] );
        exit( EXIT_FAILURE );
    }

    // Convert the provided key into bytes.
    byte key[ BLOCK_BYTES ] = {};
    prepareKey( key, argv[ CMD_KEY ] );

    // Generate the subkeys from the provided key.
    byte K[ ROUND_COUNT ][ SUBKEY_BYTES ] = {};
    generateSubkeys( K, key );

    // Open the output file.
    FILE *output = fopen( argv[ CMD_OUTPUT ], "wb" );

    // Exit as failure if the input file cannot be opened.
    if ( output == NULL ) {
        perror( argv[ CMD_OUTPUT ] );
        exit( EXIT_FAILURE );
    }

    DESBlock *block = ( DESBlock * )malloc( sizeof( DESBlock ) );
    for ( int i = 0; i < file_length / 8; i++ ) {
        readBlock( file, block );

        // Decrypt the file.
        decryptBlock( block, K );

        // If there was padding added to the end of the cipher text, decrease the length to prevent padding from being written to the file.
        for ( int i = block->len; i > 0; i-- ) {
            if ( ( block->data[ i ] | 0x00 ) == 0 ) {
                block->len--;
            }
        }

        writeBlock( output, block );
    }
}
