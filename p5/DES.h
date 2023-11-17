/**
 * @file DES.h
 * @author Ian M Brain (imbrain)
 * This file provides the core functionality for encryption and decryption byt implementing the DES algorithm
 * This functionality includes preparing the key, getting and setting bits, ecrypting and decryption block data, and additional functionality.
 * These functions are used in the ecrypt.c and decrypt.c to encrypt or decrypt blocks of data respectively and prepare subkeys.
*/

#include "DESMagic.h"
#include <stdlib.h>

/** Number of bits in a byte. */
#define BYTE_SIZE 8

/** Round a number of bits up to the nearest number of bytes needed 
    to store that many bits. */
#define ROUND_TO_BYTES( bits ) (((bits) + BYTE_SIZE - 1)/BYTE_SIZE)

/** Number of bytes in a DES block. */
#define BLOCK_BYTES ROUND_TO_BYTES( BLOCK_BITS )

/** Number of bytes in the left or right halves of a block (L and R). */
#define BLOCK_HALF_BYTES ROUND_TO_BYTES( BLOCK_HALF_BITS )

/** Number of bytes to store the left-side and right-side values (C
    and D) used to create the subkeys. */
#define SUBKEY_HALF_BYTES ROUND_TO_BYTES( SUBKEY_HALF_BITS )

/** Number of bytes to store a whole subkey (K_1 .. K_16). */
#define SUBKEY_BYTES ROUND_TO_BYTES( SUBKEY_BITS )

#ifndef DES_STRUCT
#define DES_STRUCT

/** Type used to represent a block to encrypt or decrypt with DES. */
typedef struct {
  /** Sequence of bytes in the block. */
  byte data[ BLOCK_BYTES ];

  /** Number of bytes currently in the data array (e.g., the last block in a file could
      be shorter. */
  int len;
} DESBlock;

#endif

/**
 * Copies the provided textKey to an output key.
 * If the textKey is less than eight characters in length then zeros are added to the end of the key.
 * Exit as failure if the key to longer than eight characters.
 * @param textKey key that must be less than or equal to eight bytes in length.
 * @param key key padded with zeros to be length eight.
*/
void prepareKey( byte key[ BLOCK_BYTES ], char const *textKey );

/**
 * Determines whether the bit at the specified index is zero or one.
 * This function is used to determine the bit at specified indexes in other functions.
 * @param data array of bytes to alter.
 * @param idx index of the bit to check is one or zero.
 * @return one or zero based on the value of the bit at the specified index.
*/
int getBit( byte const data[], int idx );

/**
 * Clears or sets the bit at the specified index of the data depending on whether the value is one or zero.
 * Used on other function to enable bits at specified index to be set or cleared.
 * @param data array of bytes to alter.
 * @param idx index of bit to set or clear.
 * @param val one or zero depending on whether the bit should be set or cleared.
*/
void putBit( byte data[], int idx, int val );

/**
 * Copies a specified number of bits from the input data to the output data.
 * The bit copied from input is specified by an index within the perm array.
 * The index from perm corresponds to each value in the perm array from zero to one less than n.
 * @param output data to set the bit values in.
 * @param input data to get the bit values from.
 * @param perm data to get the indexes from.
 * @param n number of bits to copy from input to output.
*/
void permute( byte output[], byte const input[], int const perm[], int n );

void generateSubkeys( byte K[ ROUND_COUNT ][ SUBKEY_BYTES ], byte const key[ BLOCK_BYTES ] );

/**
 * This function takes an output byte, and array of six bytes as input, and and index ranging from zero to seven.
 * Idx is multiplied by seven and added by one to six to get six consecutive bit values from the input data.
 * The six bits are then used to determine the row and column to look up a value from an sBox table.
 * The value specified in the sBox table is converted to binary and stored in the output data.
 * This function is used within the fFunction to each six bit value of R into a four bit output value.
 * @param output byte to store the output.
 * @param input byte data to get sbox indexes from.
 * @param idx index ranging from zero to seven.
*/
void sBox( byte output[ 1 ], byte const input[ SUBKEY_BYTES ], int idx );

/**
 * This function takes a four byte result value and input value as well as a six byte key value.
 * The input value is permuted to six bytes and modified using the key value.
 * Each six bits of this value are transformed into four bit values using sBox.
 * These are combined into four bytes and permuted into the result value.
 * @param result four byte rearranged version of the R value.
 * @param R four byte value to rearrange.
 * @param K six byte value used to further rearrange the R value.
*/
void fFunction( byte result[ BLOCK_HALF_BYTES ], byte const R[ BLOCK_HALF_BYTES ], byte const K[ SUBKEY_BYTES ] );

/**
 * Encrypts a block of eight bytes using the data of sixteen keys.
 * The block is first permuted into two 32 bit balues.
 * The encryption process then uses the fFucntion to to rearrange the bit values based on each key.
 * Used in encrypt.c to encrypt each eight blocks of data.
 * @param block block containing the eight byte data to encrypt.
 * @param K sixteen key data.
*/
void encryptBlock( DESBlock *block, byte const K[ ROUND_COUNT ][ SUBKEY_BYTES ] );

/**
 * Decrypts a block of eight bytes using the data of sixteen keys.
 * The block is first permuted into two 32 bit balues.
 * The encryption process then uses the fFucntion to to rearrange the bit values based on each key.
 * Decryption differs from encryption in that decryption uses the keys in reverse order.
 * Used in decrypt.c to decrypt each eight blocks of data.
 * @param block block containing the eight byte data to encrypt.
 * @param K sixteen key data.
*/
void decryptBlock( DESBlock *block, byte const K[ ROUND_COUNT ][ SUBKEY_BYTES ] );
