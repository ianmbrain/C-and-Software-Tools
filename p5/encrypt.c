/**
 * @file encrypt.c
 * @author Ian M Brain (imbrain)
 * This file is the main file for the encryption program.
 * The main function command line arguments include the encrypt program, encyption key, a file to encrypt, and an output file.
 * This file uses functions from DES.c to generate the subkeys and encrypt the file.
 * Also uses functions from io.c to read and write eight byte blocks of data from the input and output file.
*/

#include "io.h"
#include "DES.h"
#include <string.h>

/** Expected number of command line arguments. */
#define CMD_ARGS 4

/** Command line argument that contains the encryption key. */
#define CMD_KEY 1

/** Command line argument that contains the file name. */
#define CMD_FILE 2

/** Command line argument that contains the output file name. */
#define CMD_OUTPUT 3

/**
 * Main function used to encrypt files.
 * Takes in command line arguments of a encryption key, file to encrypt, and an output file.
 * Encrypts a file using the encryption key and writes the encrypted data to an output file.
 * Uses functions from DES.c to generate subkeys and encrypt the block.
 * Uses functions from io.c to read and write eight byte blocks of data from the input and output file.
 * Exits as a failure if there are an invalid number of command arguments, invalid input or output file, or invalid encryption key.
 * @param argc number of command line arguments.
 * @param argv array of command line argumnets values.
 * @return one if the command line arguments are invalid or zero if the program passes.
*/
int main( int argc, char *argv[] ) {
    // Exit as failure if there are an invalid number of command arguments.
    if ( argc != CMD_ARGS ) {
        fprintf( stderr, "usage: encryt <key> <input_file> <output_file>\n");
        exit( EXIT_FAILURE );
    }

    // Convert the provided key into bytes.
    byte key[ BLOCK_BYTES ] = {};
    prepareKey( key, argv[ CMD_KEY ] );

    // Open the input file.
    FILE *file = fopen( argv[ CMD_FILE ], "rb" );

    // Exit as failure if the input file cannot be opened.
    if ( file == NULL ) {
        perror( argv[ CMD_FILE ] );
        exit( EXIT_FAILURE );
    }

    // Generate the subkeys from the provided key.
    byte K[ ROUND_COUNT ][ SUBKEY_BYTES ] = {};
    generateSubkeys( K, key );

    // Open the output file.
    FILE *output = fopen( argv[ CMD_OUTPUT ], "wb" );

    // Exit as failure if the input file cannot be opened.
    if ( output == NULL ) {
        fclose( file );
        perror( argv[ CMD_OUTPUT ] );
        exit( EXIT_FAILURE );
    }

    // Initialize the DESBlock.
    DESBlock *block = ( DESBlock * )malloc( sizeof( DESBlock ) );

    // Determine the length of the file.
    int file_length = 0;
    fseek( file, 0, SEEK_END );
    file_length = ftell( file );
    fseek( file, -file_length, SEEK_END );

    // Set the file length to the next multiple of eight larger than the file length if the file is not a multiple of eight.
    if ( file_length % BYTE_SIZE != 0 ) {
        int c = 0;
        c = file_length / BYTE_SIZE;
        file_length = ( c + 1 ) * BYTE_SIZE;
    }

    for ( int i = 0; i < file_length / BYTE_SIZE; i++ ) {
        // Read eight bytes of data from the file to the block.
        readBlock( file, block );

        // Add padding to the last block if the file legnth was not a multiple of eight.
        if ( block->len != BYTE_SIZE ) {
            int pad_count = BYTE_SIZE - block->len;

            for ( int b = 0; b < pad_count; b++ ) {
                block->data[ block->len + b ] =  0x00;
            }

            block->len = BYTE_SIZE;

        }

        // Encrypt the block data.
        encryptBlock( block, K );

        // Write the block data to the output file.
        writeBlock( output, block );
    }

    // Free the block memory and close the files.
    free( block );
    fclose( file );
    fclose( output );
}
