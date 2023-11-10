/** 
    @file DES.c
    @author
    Implementation of the DES algorithm.
*/

// This is where the DES algorithms for encryption and decryption are implemented. 
// It’s broken up into several small functions for the various steps, so these can be tested and debugged independently

#include "DES.h"
#include "DESMagic.h"

struct DESBlockStruct {
    unsigned char type;
    int len;
} typedef DESBlock;

void prepareKey( byte key[ BLOCK_BYTES ], char const *textKey ) {

}

int getBit( byte const data[], int idx ) {

}

void putBit( byte data[], int idx, int val ) {

}

void permute( byte output[], byte const input[], int const perm[], int n ) {

}

void generateSubkeys( byte K[ ROUND_COUNT ][ SUBKEY_BYTES ], byte const key[ BLOCK_BYTES ] ) {

}

void sBox( byte output[ 1 ], byte const input[ SUBKEY_BYTES ], int idx ) {

}

void fFunction( byte result[ BLOCK_HALF_BYTES ], byte const R[ BLOCK_HALF_BYTES ], byte const K[ SUBKEY_BYTES ] ) {

}

void encryptBlock( DESBlock *block, byte const K[ ROUND_COUNT ][ SUBKEY_BYTES ] ) {

}

void decryptBlock( DESBlock *block, byte const K[ ROUND_COUNT ][ SUBKEY_BYTES ] ) {

}
