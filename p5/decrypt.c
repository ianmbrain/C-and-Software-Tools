/**
 * @file decrypt.c
 * @author Ian M Brain (imbrain)
 * This file is the main file for the decryption program.
 * The main function command line arguments include the decrypt program, decryption key, a file to decrypt, and an output file.
 * This file uses functions from DES.c to generate the subkeys and decrypt the file.
 * Also uses functions from io.c to read and write eight byte blocks of data from the input and output file.
*/

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

/**
 * Main function used to decrypt files.
 * Takes in command line arguments of a decryption key, file to decrypt, and an output file.
 * Decrypts a file using the decryption key and writes the decrypted data to an output file.
 * Uses functions from DES.c to generate subkeys and encrypt the block.
 * Uses functions from io.c to read and write eight byte blocks of data from the input and output file.
 * Exits as a failure if there are an invalid number of command arguments, invalid input or output file, 
 * invalid encryption key, or if the input file length is not a multiple of eight.
 * @param argc number of command line arguments.
 * @param argv array of command line argumnets values.
 * @return one if the command line arguments are invalid or zero if the program passes.
*/
int main( int argc, char *argv[] ) {
    // Exit as failure if there are an invalid number of command arguments.
    if ( argc != CMD_ARGS ) {
        fprintf( stderr, "usage: decrypt <key> <input_file> <output_file>\n");
        exit( EXIT_FAILURE );
    }

    // Convert the provided key into bytes.
    byte key[ BLOCK_BYTES ] = {};
    prepareKey( key, argv[ CMD_KEY ] );

    // Generate the subkeys from the provided key.
    byte K[ ROUND_COUNT ][ SUBKEY_BYTES ] = {};
    generateSubkeys( K, key );

    // Open the input file.
    FILE *file = fopen( argv[ CMD_FILE ], "rb" );

    // Exit as failure if the input file cannot be opened.
    if ( file == NULL ) {
        perror( argv[ CMD_FILE ] );
        exit( EXIT_FAILURE );
    }

    // Determine the length of the input file.
    int file_length = 0;
    fseek( file, 0, SEEK_END );
    file_length = ftell( file );
    fseek( file, -file_length, SEEK_END );

    // Exit as failure if the decryption file length is not a multiple of eight.
    if ( file_length % BYTE_SIZE != 0 ) {
        fclose( file );
        fprintf( stderr, "%s%s\n", "Bad ciphertext file length: ", argv[ CMD_FILE ] );
        exit( EXIT_FAILURE );
    }

    // Open the output file.
    FILE *output = fopen( argv[ CMD_OUTPUT ], "wb" );

    // Exit as failure if the input file cannot be opened.
    if ( output == NULL ) {
        perror( argv[ CMD_OUTPUT ] );
        exit( EXIT_FAILURE );
    }

    // Allocate memory to the block
    DESBlock *block = ( DESBlock * )malloc( sizeof( DESBlock ) );

    for ( int i = 0; i < file_length / BYTE_SIZE; i++ ) {
        // Read the eight bytes of data from the file to the block.
        readBlock( file, block );

        // Decrypt the block data.
        decryptBlock( block, K );

        // If there was padding added to the end of the cipher text, decrease the length to prevent padding from being written to the file.
        for ( int i = block->len; i > 0; i-- ) {
            if ( ( block->data[ i ] | 0x00 ) == 0 ) {
                block->len--;
            }
        }

        // Write the block of data to the output file.
        writeBlock( output, block );
    }

    // Free the block memory and close the files.
    free( block );
    fclose( file );
    fclose( output );
}
