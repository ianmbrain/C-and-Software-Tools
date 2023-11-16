
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

int main( int argc, char *argv[] ) {
    // Exit as failure if there are an invalid number of command arguments.
    if ( argc != CMD_ARGS ) {
        fprintf( stderr, "usage: encryt <key> <input_file> <output_file>\n");
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

    int file_length = 0;
    fseek( file, 0, SEEK_END );
    file_length = ftell( file );
    fseek( file, -file_length, SEEK_END );

    if ( file_length % 8 != 0 ) {
        int c = 0;
        c = file_length / 8;
        file_length = ( c + 1 ) * 8;
    }

    for ( int i = 0; i < file_length / 8; i++ ) {
        readBlock( file, block );

        if ( block->len != 8 ) {
            int pad_count = 8 - block->len;

            for ( int b = 0; b < pad_count; b++ ) {
                block->data[ block->len + b ] =  0x00;
            }

            block->len = 8;

        }

        // Encrypt the file.
        encryptBlock( block, K );

        writeBlock( output, block );
    }
}
