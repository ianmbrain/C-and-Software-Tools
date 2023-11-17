/**
 * @file DES.c
 * @author Ian M Brain (imbrain)
 * This file provides the core functionality for encryption and decryption byt implementing the DES algorithm
 * This functionality includes preparing the key, getting and setting bits, ecrypting and decryption block data, and additional functionality.
 * These functions are used in the ecrypt.c and decrypt.c to encrypt or decrypt blocks of data respectively and prepare subkeys.
*/

#include "DESMagic.h"
#include "DES.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** Value of the combined permutated halves of the key used withing generateSubkeys(). */
#define GENERATE_COMBINED_KEYS 7

/** Number of bits in half a byte. */
#define HALF_BYTE 4

/** Bit value used to clear specific bits. */
#define CLEAR_BITS 0xFEFF

/** Bit value used to set specific bits. */
#define SET_BITS 0x0100

/** Value to adjust the index by when calulating which byte the index is within. */
#define INDEX_ADJUSTMENT 1

void prepareKey( byte key[ BLOCK_BYTES ], char const *textKey ) {
    // Length of the key
    int key_len = strlen( textKey );

    // Exit as failure if the encryption key is longer than eight characters.
    if ( key_len > BYTE_SIZE ) {
        fprintf( stderr, "Key too long\n" );
        exit( EXIT_FAILURE );
    }

    // Copy the characters of the text key into the key
    for ( int i = 0; i < key_len; i++ ) {
        key[ i ] = textKey[ i ];
    }

    // Pad the key with zeros if the length is under eight characters.
    if ( key_len < BYTE_SIZE ) {
        while ( key_len < BYTE_SIZE ) {
            key[ key_len ] = 0;
            key_len++;
        }
    }
}

int getBit( byte const data[], int idx ) {
    // Index of the byte idx is within.
    int byte_index = 0;
    byte_index = (idx - INDEX_ADJUSTMENT) / BYTE_SIZE;

    // Bit position of idx within a byte.
    int bit_index = 0;
    bit_index = idx % BYTE_SIZE;
    // If the bit index is zero it indicates idx is a multiple of eith and should thus be the eigth index of the byte.
    if ( bit_index == 0 )
        bit_index = BYTE_SIZE;

    // Moves the one bit to the position of bit_index and returns whether the bit is one or zero.
    return ( data[ byte_index ] & ( SET_BITS >> bit_index ) ) != 0;
}

void putBit( byte data[], int idx, int val ) {
    int byte_index = (idx - INDEX_ADJUSTMENT) / BYTE_SIZE;
    int bit_index = idx % BYTE_SIZE;

    // If the bit index is zero it indicates idx is a multiple of eith and should thus be the eigth index of the byte.
    if ( bit_index == 0 ) {
        bit_index = BYTE_SIZE;
    }

    // Use the bitwise and operator to clear the bit at the index if the value to set is zero.
    if ( val == 0 ) {
        data[ byte_index ] = data[ byte_index ] & ( CLEAR_BITS >> bit_index );
    }
    // Use the bitwise or operator to set the bit at the index if the value to set is one.
    else
        data[ byte_index ] = data[ byte_index ] | ( SET_BITS >> bit_index );  
}

void permute( byte output[], byte const input[], int const perm[], int n ) {
    // Index based on the perm value.
    int p_idx = 0;

    for ( int i = 1; i <= n; i++ ) {
        p_idx = 0;
        p_idx = perm[ i - 1 ];

        // Get the bit at the perm specified index from the input data.
        int value = getBit( input, p_idx );
        // Set that bit value to the correct index in output.
        putBit( output, ( i ), value );

        // Set the remaining bits of the last byte to 0 if n isn't a multiple of eight.
        if( ( n % BYTE_SIZE ) != 0 ) {
            // Value of the index raised to the next highest multiple of eight.
            int even_byte = ( ( n / BYTE_SIZE ) + 1 ) * BYTE_SIZE;
            for ( int i = n + 1; i <= even_byte; i++ ) {
                putBit( output, ( i ), 0 );
            }
        }
    }
}

void generateSubkeys( byte K[ ROUND_COUNT ][ SUBKEY_BYTES ], byte const key[ BLOCK_BYTES ] ) {
    // Create the left permutation from the given key.
    byte left_perm[ SUBKEY_HALF_BYTES ];
    permute( left_perm, key, leftSubkeyPerm, SUBKEY_HALF_BITS );

    // Create the right permutation from the given key.
    byte right_perm[ SUBKEY_HALF_BYTES ];
    permute( right_perm, key, rightSubkeyPerm, SUBKEY_HALF_BITS );

    // Bits move in the left permutation.
    int c_bit_one = 0;
    int c_bit_two = 0;
    int c_bit_three = 0;
    int c_bit_four = 0;

    // Bits move in the right permutation.
    int d_bit_one = 0;
    int d_bit_two = 0;
    int d_bit_three = 0;
    int d_bit_four = 0;

    for ( int i = 1; i < ROUND_COUNT; i++ ) {
        if ( subkeyShiftSchedule[ i ] == 1 ) {
            // Get the first bit from each byte.
            int get_index = 1;
            c_bit_one = getBit( left_perm, get_index );
            d_bit_one = getBit( right_perm, get_index );

            get_index += BYTE_SIZE;
            c_bit_two = getBit( left_perm, get_index );
            d_bit_two = getBit( right_perm, get_index );

            get_index += BYTE_SIZE;
            c_bit_three = getBit( left_perm, get_index );
            d_bit_three = getBit( right_perm, get_index );

            get_index += BYTE_SIZE;
            c_bit_four = getBit( left_perm, get_index );
            d_bit_four = getBit( right_perm, get_index );

            // Shift each byte to the left by one.
            for ( int i = 0; i < SUBKEY_HALF_BYTES; i++ ) {
                left_perm[ i ] = left_perm[ i ] << 1;
                right_perm[ i ] = right_perm[ i ] << 1;
            }

            // Put each bit in the correct place.
            int put_index = BYTE_SIZE;
            putBit( left_perm, put_index, c_bit_two );
            putBit( right_perm, put_index, d_bit_two );

            put_index += BYTE_SIZE;
            putBit( left_perm, put_index, c_bit_three );
            putBit( right_perm, put_index, d_bit_three );

            put_index += BYTE_SIZE;
            putBit( left_perm, put_index, c_bit_four );
            putBit( right_perm, put_index, d_bit_four );

            put_index += HALF_BYTE;
            putBit( left_perm, put_index, c_bit_one );
            putBit( right_perm, put_index, d_bit_one );
        }
        // If the shift schedule is two, get two bits.
        else {
            for ( int j = 0; j <= 1; j++ ) {
                // Get the first bit from each byte.
                int get_index = 1;
                c_bit_one = getBit( left_perm, get_index );
                d_bit_one = getBit( right_perm, get_index );

                get_index += BYTE_SIZE;
                c_bit_two = getBit( left_perm, get_index );
                d_bit_two = getBit( right_perm, get_index );

                get_index += BYTE_SIZE;
                c_bit_three = getBit( left_perm, get_index );
                d_bit_three = getBit( right_perm, get_index );

                get_index += BYTE_SIZE;
                c_bit_four = getBit( left_perm, get_index );
                d_bit_four = getBit( right_perm, get_index );

                // Shift each byte to the left by one.
                for ( int i = 0; i < SUBKEY_HALF_BYTES; i++ ) {
                    left_perm[ i ] = left_perm[ i ] << 1;
                    right_perm[ i ] = right_perm[ i ] << 1;
                }

                // Put each bit in the correct place.
                int put_index = BYTE_SIZE;
                putBit( left_perm, put_index, c_bit_two );
                putBit( right_perm, put_index, d_bit_two );

                put_index += BYTE_SIZE;
                putBit( left_perm, put_index, c_bit_three );
                putBit( right_perm, put_index, d_bit_three );

                put_index += BYTE_SIZE;
                putBit( left_perm, put_index, c_bit_four );
                putBit( right_perm, put_index, d_bit_four );

                put_index += HALF_BYTE;
                putBit( left_perm, put_index, c_bit_one );
                putBit( right_perm, put_index, d_bit_one );
            }
        }

        // Combine the subkeys into one 56 bit key.
        // When combining the last four bits in each key must be removed to make them truly 28 bits
        byte left_right_key[ GENERATE_COMBINED_KEYS ];

        // Copy bytes from the left perm.
        for ( int b = 0; b < BLOCK_HALF_BYTES; b++ ) {
            if ( b < ( BLOCK_HALF_BYTES - 1 ) ) {
                left_right_key[ b ] = left_perm[ b ];
            }
            // On the last by, get the last four bits from the left perm and the first four bits from the second perm.
            else {
                byte last_byte = 0x00;

                last_byte = last_byte | left_perm[ b ];

                last_byte = last_byte | ( right_perm[ 0 ] >> BLOCK_HALF_BYTES );

                left_right_key[ b ] = last_byte;
            }
        }

        // Copy the bytes from the right perm. Each byte is a combination of the last and first four bits from sequential bytes.
        for ( int b = BLOCK_HALF_BYTES; b <= SUBKEY_BYTES; b++ ) {
            byte last_byte = 0x00;

            last_byte = last_byte | ( right_perm[ b - BLOCK_HALF_BYTES ] << BLOCK_HALF_BYTES );

            last_byte = last_byte | ( right_perm[ b - ( BLOCK_HALF_BYTES - 1 ) ] >> BLOCK_HALF_BYTES );

            left_right_key[ b ] = last_byte;
        }

        // Permute the 56 bit key combination into a 48 bit key.
        byte output_key[ SUBKEY_BYTES ];
        permute( output_key, left_right_key, subkeyPerm, SUBKEY_BITS );

        // Add the permuted 48 bit key to the K array.
        for ( int b = 0; b < SUBKEY_BYTES; b++ ) {
            K[ i ][ b ] = output_key[ b ];
        }
    }
}

void sBox( byte output[ 1 ], byte const input[ SUBKEY_BYTES ], int idx ) {
    // Adjusted index of the input value.
    int index = idx * SBOX_INPUT_BITS;
    byte b1[ SBOX_INPUT_BITS ] = {};

    for ( int i = 0; i < SBOX_INPUT_BITS; i++ ) {
        index += 1;

        // Which byte of input the index is in.
        int input_byte = ( index - 1 ) / BYTE_SIZE;

        // Which bit of the input byte the index is.
        int input_bit = index % BYTE_SIZE;
        if ( input_bit == 0 )
            input_bit = BYTE_SIZE;

        // Move the bit to the lowest order bit.
        byte input_value = ( input[ input_byte ] >> ( BYTE_SIZE - input_bit ) ) & 0x01;

        // Set the corresponding array value to the value of the bit.
        b1[ i ] = input_value;
    }

    // Determine the row and column values
    int row = ( (b1[ 0 ] << 1) | b1[ 5 ] );
    int col = ( (b1[ 1 ] << 3 ) | 
                (b1[ 2 ] << 2 ) | 
                (b1[ 3 ] << 1 ) | 
                (b1[ 4 ]) );

    // Determine what index of sbox to use.
    index = idx * SBOX_INPUT_BITS + 1;
    int sbox_index = (index - 1) / SBOX_INPUT_BITS;

    int sbox_output = sBoxTable[ sbox_index ][ row ][ col ];

    // Convert the sbox integer to binary.
    byte output_byte[ 1 ] = { 0x00 };
    int test_bit = BYTE_SIZE;

    while ( test_bit != 0 ) {
        if ( sbox_output / test_bit != 0 ) {
            if ( test_bit == 8 ) {
                putBit( output_byte, 1, 1 );
            }
            else if ( test_bit == 4 ) {
                putBit( output_byte, 2, 1 );
            }
            else if ( test_bit == 2 ) {
                putBit( output_byte, 3, 1 );
            }
            else if ( test_bit == 1 ) {
                putBit( output_byte, 4, 1 );
            }
            sbox_output -= test_bit;

        }

        test_bit /= 2;
    }

    // Set the output byte to the transformed sbox value.
    output[ 0 ] = output_byte[ 0 ];
}

void fFunction( byte result[ BLOCK_HALF_BYTES ], byte const R[ BLOCK_HALF_BYTES ], byte const K[ SUBKEY_BYTES ] ) {
    // Create the expanded r value using a 48 bit permutation.
    byte expanded_r[ SUBKEY_BYTES ];
    permute( expanded_r, R, expandedRSelector, SUBKEY_BITS );

    // Combine expanded r and K using exclusive or operation.
    byte B[ SUBKEY_BYTES ];
    for ( int i = 0; i < SUBKEY_BYTES; i++ ) {
        B[ i ] = expanded_r[ i ] ^ K[ i ];
    }

    // Result of each sBox() calculation.
    byte sbox_bits[ 1 ];
    // Cumulative result of the sBox calculations.
    byte sbox_result[ BLOCK_HALF_BYTES ];

    for ( int i = 0; i < BYTE_SIZE; i++ ) {
        // sBox() each six bits of B into four bit values.
        sBox( sbox_bits, B, i);

        int val = 0;
        for ( int b = 1; b <= SBOX_ROWS; b++ ) {
            // Obtain each bit from the four bit value.
            val = getBit( sbox_bits, b );
            // Put that bit value at the correct index in the result.
            putBit( sbox_result, ( SBOX_ROWS * i + b ), val );
        }
    }

    // Permute the result into the correct result.
    permute( result, sbox_result, fFunctionPerm, BLOCK_HALF_BITS );
}

void encryptBlock( DESBlock *block, byte const K[ ROUND_COUNT ][ SUBKEY_BYTES ] ) {
    byte L[ BLOCK_HALF_BYTES ];
    byte R[ BLOCK_HALF_BYTES ];
    byte l_next[ BLOCK_HALF_BYTES ];
    byte r_next[ BLOCK_HALF_BYTES ];

    permute( L, block->data, leftInitialPerm, BLOCK_HALF_BITS);
    permute( R, block->data, rightInitialPerm, BLOCK_HALF_BITS );

    for ( int i = 1; i < ROUND_COUNT; i++ ) {
        // Perform the fFunction() on R using the corresponding K value.
        fFunction( r_next, R, K[ i ] );

        // Copy R into l_next.
        for ( int b = 0; b < BLOCK_HALF_BYTES; b++ )
            l_next[ b ] = R[ b ];

        // Set R equal to the exclusive or value of r_next and L.
        for ( int b = 0; b < BLOCK_HALF_BYTES; b++ )
            R[ b ] = r_next[ b ] ^ L[ b ];

        // Set L to the previous value of R, which is l_next.
        for ( int b = 0; b < BLOCK_HALF_BYTES; b++ )
            L[ b ] = l_next[ b ];
    }

    // Combine R and L.
    byte final[ BLOCK_BYTES ];
    for ( int i = 0; i < BYTE_SIZE; i++ ) {
        if ( i < BLOCK_HALF_BYTES )
            final[ i ] = R[ i ];
        else
            final[ i ] = L[ i - BLOCK_HALF_BYTES ];
    }

    // Permute this final value back into the result block.
    permute( block->data, final, finalPerm, BLOCK_BITS );
}

void decryptBlock( DESBlock *block, byte const K[ ROUND_COUNT ][ SUBKEY_BYTES ] ) {
    byte L[ BLOCK_HALF_BYTES ];
    byte R[ BLOCK_HALF_BYTES ];
    byte l_next[ BLOCK_HALF_BYTES ];
    byte r_next[ BLOCK_HALF_BYTES ];

    permute( L, block->data, leftInitialPerm, BLOCK_HALF_BITS );
    permute( R, block->data, rightInitialPerm, BLOCK_HALF_BITS );

    for ( int i = ROUND_COUNT - 1; i >= 1; i-- ) {
        // Perform the fFunction() on R using the corresponding K value.
        fFunction( r_next, R, K[ i ] );

        // Copy R into l_next.
        for ( int b = 0; b < BLOCK_HALF_BYTES; b++ )
            l_next[ b ] = R[ b ];

        // Set R equal to the exclusive or value of r_next and L.
        for ( int b = 0; b < BLOCK_HALF_BYTES; b++ )
            R[ b ] = r_next[ b ] ^ L[ b ];

        // Set L to the previous value of R, which is l_next.
        for ( int b = 0; b < BLOCK_HALF_BYTES; b++ )
            L[ b ] = l_next[ b ];
    }

    // Combine R and L.
    byte final[ BLOCK_BYTES ];
    for ( int i = 0; i < BYTE_SIZE; i++ ) {
        if ( i < BLOCK_HALF_BYTES )
            final[ i ] = R[ i ];
        else
            final[ i ] = L[ i - BLOCK_HALF_BYTES ];
    }

    // Permute this final value back into the result block.
    permute( block->data, final, finalPerm, BLOCK_BITS );
}
