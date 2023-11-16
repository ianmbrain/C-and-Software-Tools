
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
        fprintf( stderr, "usage: decrypt <key> <input_file> <output_file>\n");
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

    // If the decryption file length is not a multiple of eight exit as failure.
    int file_length = 0;
    fseek( file, 0, SEEK_END );
    file_length = ftell( file );
    //fprintf( stderr, "\n%d\n", file_length);
    fseek( file, -file_length, SEEK_END );

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

    DESBlock *block = ( DESBlock * )malloc( sizeof( DESBlock ) );
    FILE *output = fopen( argv[ CMD_OUTPUT ], "wb" );
    for ( int i = 0; i < file_length / 8; i++ ) {
        readBlock( file, block );

        // Decrypt the file.
        decryptBlock( block, K );
        // for ( int b = 0; b < 8; b++ ) {
        //     fprintf( stderr, "\n%x", block->data[b] );
        // }
        
        //fseek( output, i * 8, SEEK_CUR );
        // fprintf( stderr, "\n\n%d\n", block->len );
        writeBlock( output, block );
    }
    // DESBlock *block = ( DESBlock * )malloc( sizeof( DESBlock ) );

    // readBlock( file, block );

    // // Convert the provided key into bytes.
    // byte key[ BLOCK_BYTES ] = {};
    // prepareKey( key, argv[ CMD_KEY ] );

    // // Generate the subkeys from the provided key.
    // byte K[ ROUND_COUNT ][ SUBKEY_BYTES ] = {};
    // generateSubkeys( K, key );

    // // Encrypt the file.
    // decryptBlock( block, K );

    // Write the encrypted file to the provided output file.
    // FILE *output = fopen( argv[ CMD_OUTPUT ], "wb" );
    // writeBlock( output, block );
}
