/** 
    @file DES.h
    @author
    Header for the DES Implementation.
*/

#include "DESMagic.h"

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

/** Type used to represent a block to encrypt or decrypt with DES. */
typedef struct {
  /** Sequence of bytes in the block. */
  byte data[ BLOCK_BYTES ];

  /** Number of bytes currently in the data array (e.g., the last block in a file could
      be shorter. */
  int len;
} DESBlock;

void prepareKey( byte key[ BLOCK_BYTES ], char const *textKey );

int getBit( byte const data[], int idx );

void putBit( byte data[], int idx, int val );

void permute( byte output[], byte const input[], int const perm[], int n );

void generateSubkeys( byte K[ ROUND_COUNT ][ SUBKEY_BYTES ], byte const key[ BLOCK_BYTES ] );

void sBox( byte output[ 1 ], byte const input[ SUBKEY_BYTES ], int idx );

void fFunction( byte result[ BLOCK_HALF_BYTES ], byte const R[ BLOCK_HALF_BYTES ], byte const K[ SUBKEY_BYTES ] );

void encryptBlock( DESBlock *block, byte const K[ ROUND_COUNT ][ SUBKEY_BYTES ] );
