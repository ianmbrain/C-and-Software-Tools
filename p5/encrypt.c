
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

// -__________-----------------------------------------------
    // Open the file to encrypt.
    // FILE *file = fopen( argv[ CMD_FILE ], "rb" );

    // If the file cannot be encrypted exit as failure.
    // if ( file == NULL ) {
    //     perror( argv[ CMD_FILE ] );
    //     exit( EXIT_FAILURE );
    // }

    // // If the encryption key is longer than allowed exit as failure.
    // if ( strlen( argv[ CMD_KEY ] ) > 8 ) {
    //     fprintf( stderr, "Key too long\n");
    //     exit( EXIT_FAILURE );
    // }

    // DESBlock *block = ( DESBlock * )malloc( sizeof( DESBlock ) );

    // readBlock( file, block );

    // Convert the provided key into bytes.
    // byte key[ BLOCK_BYTES ] = {};
    // prepareKey( key, argv[ CMD_KEY ] );

    // Generate the subkeys from the provided key.
    // byte K[ ROUND_COUNT ][ SUBKEY_BYTES ] = {};
    // generateSubkeys( K, key );

    // Encrypt the file.
    // encryptBlock( block, K );

    // Write the encrypted file to the provided output file.
    // FILE *output = fopen( argv[ CMD_OUTPUT ], "wb" );
    // writeBlock( output, block );

    // _______----------------------------------------------------

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

    // DESBlock *block = ( DESBlock * )malloc( sizeof( DESBlock ) );

    // readBlock( file, block );
    // printBits( block->data, (8*8) );

    // Convert the provided key into bytes.
    byte key[ BLOCK_BYTES ] = {};
    prepareKey( key, argv[ CMD_KEY ] );

    // Generate the subkeys from the provided key.
    byte K[ ROUND_COUNT ][ SUBKEY_BYTES ] = {};
    generateSubkeys( K, key );

    DESBlock *block = ( DESBlock * )malloc( sizeof( DESBlock ) );
    FILE *output = fopen( argv[ CMD_OUTPUT ], "wb" );

    int file_length = 0;
    fseek( file, 0, SEEK_END );
    file_length = ftell( file );
    //fprintf( stderr, "\n%d\n", file_length);
    fseek( file, -file_length, SEEK_END );
    // fprintf( stderr, "\n%d\n", file_length);

    if ( file_length % 8 != 0 ) {
        int c = 0;
        c = file_length / 8;
        file_length = ( c + 1 ) * 8;
        //  fprintf( stderr, "\nHERE\n");
    }

    for ( int i = 0; i < file_length / 8; i++ ) {
        readBlock( file, block );

        // fprintf( stderr, "\n%d\n", block->len);
        // fprintf( stderr, "\n%d\n", file_length);
        // for ( int b = 0; b < block->len ; b++ ) {
        //     fprintf( stderr, "\n%x\n", block->data[ b ]);
        // }
        // fprintf( stderr, "\n%d-------------------\n", block->len );

        if ( block->len != 8 ) {
            // fprintf( stderr, "\n%d\n", block->len);
            int pad_count = 8 - block->len;
            int len = block->len;

            for ( int b = 0; b < pad_count; b++ ) {
                block->data[ len + b ] =  0x00;
                // fprintf( stderr, "\n%x\n", block->data[ len + b ] );
                // fprintf( stderr, "    %d\n", block->len );
            }
            // fprintf( stderr, "\n%d\n", block->len);

            // fprintf( stderr, "\n%x\n", block->data[ 5 ]);
            // fprintf( stderr, "\n%x\n", block->data[ 6 ]);
            // fprintf( stderr, "\n%x\n", block->data[ 7 ]);
            // fprintf( stderr, "\n%x\n", block->data[ 8 ]);
            block->len = 8;
            // fprintf( stderr, "\n%x\n", block->data[ 5 ]);
            // fprintf( stderr, "\n%x\n", block->data[ 6 ]);
            // fprintf( stderr, "\n%x\n", block->data[ 7 ]);

        }

        // for ( int b = 0; b < block->len ; b++ ) {
        //     fprintf( stderr, "\n%x\n", block->data[ b ]);
        // }
        // fprintf( stderr, "\n%d-------------------\n", i );

        // Encrypt the file.
        encryptBlock( block, K );

        // printBits( block->data, 8);

        writeBlock( output, block );
    }



    // printBits( block->data, (8*8) );

    // FILE *testout = fopen( "cipher-a.bin", "rb" );
    // DESBlock *blockout = ( DESBlock * )malloc( sizeof( DESBlock ) );
    // readBlock( testout, blockout );
    // printBits( blockout->data, (8*8) );
    // fclose (testout);

    // printBits( block->data, 10 );
}
