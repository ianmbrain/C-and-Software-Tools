/** 
    @file DES.c
    @author
    Implementation of the DES algorithm.
*/

// This is where the DES algorithms for encryption and decryption are implemented. 
// It’s broken up into several small functions for the various steps, so these can be tested and debugged independently

#include "DESMagic.h"
#include "DES.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** Value to adjust the index by when calulating which byte the index is within. */
#define INDEX_ADJUSTMENT 1

/**
 * Copies the provided textKey to an output key.
 * If the textKey is less than eight characters in length then zeros are added to the end of the key.
 * Exit as failure if the key to longer than eight characters.
 * @param textKey key that must be less than or equal to eight bytes in length.
 * @param key key padded with zeros to be length eight.
*/
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

/**
 * Determines whether the bit at the specified index is zero or one.
 * This function is used to determine the bit at specified indexes in other functions.
 * @param data array of bytes to examine.
 * @param idx index of the bit to check is one or zero.
 * @return one or zero based on the value of the bit at the specified index.
*/
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

    // Index used to determine what index should be checked based on bit_index.
    int b_index = 0;
    b_index = BYTE_SIZE - bit_index;

    // Moves the one bit to the position of bit_index and returns whether the bit is one or zero.
    return ( data[ byte_index ] & ( 0x01 << b_index ) ) != 0;
}

void putBit( byte data[], int idx, int val ) {
    int a_index = (idx - 1) / 8;
    //fprintf( stderr, "\n%x\n", data[ a_index ]);
    int norm_index = idx;
    if ( idx > 8 ) {
        norm_index = idx % 8;
    }
    // If idx is a multiple of 8 the modulo will make it 0. This value is really the 8th index.
    if ( norm_index == 0 ) {
        norm_index = 8;
    }

    int b_index = 8 - norm_index;

    // Problem is that it is being left shifted but then 0s are being added in its place
    // make it signed and perform a right shift?
    if ( val == 0 ) {
        // if ( idx == 1 )
        //     data[ a_index ] = data[ a_index ] & 0x7f;
        // else if ( idx == 2 )
        //     data[ a_index ] = data[ a_index ] & 0xbf;
        // else
        //     data[ a_index ] = data[ a_index ] & ( (signed char) 0xbf >> ( norm_index - 2 ) );
        data[ a_index ] = data[ a_index ] & ( 0xFEFF >> norm_index );
    }
    else
        data[ a_index ] = data[ a_index ] | ( 0x01 << b_index );  
    //fprintf( stderr, "\n%x\n", data[a_index] );

}

void permute( byte output[], byte const input[], int const perm[], int n ) {
    // copying bits from input based on each value in perm
    // if n is not a multiple of 8, the remaining bits will be 0

    // loop through perm
    // on each loop, find what array index in input/output the element is at
    // also find the normalized index
    // get what binary value to copy from the input array based on the perm value index
    // get copy that value to the output array
    // don't forget to pad with 0s

    // for loop with 0 to n. gets the first n bits based on the perm index from index array and puts them in output
        // get perm index at index i
        // find that value in input array (is it 0 or 1)
    // them set in output to 0 or 1. 
        // need to do a similar thing to divide up input/output into indexes and possible normalize n to be 1-8
        // REMEMBER - i is 0 to 1.
    // do padding.
    
    int p_idx = 0;
    int p_byte = 0;
    int i_idx = 0;
    //unsigned char i_value = 0x00;
    // for ( int b = 0; b < 8; b++ ) {
    //     fprintf( stderr, "\n%x\n", input[ b ] );
    // }

    


    for ( int i = 0; i < n; i++ ) {
        p_idx = 0;
        p_byte = 0;
        i_idx = 0;
        //i_value = 0x00;
        p_idx = perm[ i ];

        // What byte of the input array the value is in.
        p_byte =  ( p_idx - 1 ) / 8;

        // Normalize the perm index value to be between 1 - 8.
        if ( p_idx > 8 )
            i_idx = p_idx % 8;
        else
            i_idx = p_idx;

        if ( i_idx == 0 ) {
            i_idx = 8;
        }

        // Get the value at the perm index value in the input array.
        unsigned char temp_mask = 0x80;

        // temp_mask is either 0 or 1 depending on the value of the bit at the perm index value.
        temp_mask = input[ p_byte ] & ( temp_mask >> ( i_idx - 1 ) );

        // If the temp_mask value is 0 the set the bit at the first index of the output array to 0.
        if ( ( temp_mask || 0x00 ) == 0 ) {
            // i + 1 is treated as the index of output to set. Short is used to fit 16 bits.
            unsigned short output_value = 0xFEFF;

            int output_idx = 0;
            if ( ( i + 1 ) > 8 )
                output_idx = ( i + 1 ) % 8;
            else
                output_idx = ( i + 1 );

            if ( output_idx == 0 )
                output_idx = 8;

            output_value = output_value >> output_idx;

            int output_byte = i / 8;

            output[ output_byte ] = output[ output_byte ] & ( unsigned char ) output_value;
        }
        else if ( ( temp_mask || 0x00 ) == 1 ) {
            unsigned short output_value = 0x0100;

            int output_idx = 0;
            if ( ( i + 1 ) > 8 )
                output_idx = ( i + 1 ) % 8;
            else
                output_idx = ( i + 1 );

            if ( output_idx == 0 )
                output_idx = 8;

            output_value = output_value >> output_idx;

            int output_byte = i / 8;

            output[ output_byte ] = output[ output_byte ] | ( unsigned char ) output_value;
        }
        // if ( i == 8 ) {
        //     fprintf( stderr, "\n------%s\n", "HERE");
        // }
        // fprintf( stderr, "\n%x\n", output[ 0 ]);
        // fprintf( stderr, "%x\n", output[ 1 ]);

        // Set the remaining bits of the last byte to 0 if n isn't a multiple of 8.
        if ( ( n % 8 ) != 0 ) {
            // Ex: n = 9
            // 16 - 9 = 7 bits to turn to 0.
            // 9 - 8 = 1 bit to remain set as normal (1 or 0)
                // create a mask with all 0s except first bits that were already set.
                // & this mask with the second byte

            unsigned short output_value = 0xFF00;

            int last_byte = ( n / 8 );

            output_value = output_value >> ( n - ( 8 * last_byte ) );

            output[ last_byte ] = output[ last_byte ] & ( unsigned char ) output_value;
            //fprintf( stderr, "\n%d\n", n);
        }

    }

//     int a_index = 0;
//     int idx = 0;
//     unsigned char i_index = 0x80;
//     unsigned char i_value = 0;
// //    output[ 0 ] = 0x00;
// //     output[ 1 ] = 0x00; 

//     for ( int i = 0; i < n; i++ ) {
//         idx = perm[ i ] % 8;
//         if ( idx == 0 ) {
//             idx = 8;
//         }
//         //fprintf( stderr, "\n%d\n", idx);
//         //a_index = ( n - 1 ) / 8;
//         //a_index = i / 8;
//         a_index = perm[ i ] / 8;
//         i_value = input[ a_index ];
//         // fprintf( stderr, "\n%x\n", i_value);

//         // Get the binary value from input based on the perm index value.
//         i_index = i_index >> ( idx - 1 );
//         // i_value = input[ i ] & i_index;
//         //fprintf( stderr, "\n%x\n", i_index);
//         i_value = i_value & i_index;
//         //fprintf( stderr, "\n%x\n", i_value);
//         // Convert the i_value to binary 0 or 1. Copy the input array binary value to the output array at the correct index.
//         fprintf( stderr, "\n-------%d\n", i / 8);
//         if ( ( i_value && 0xFF) == 1) {
//             i_value = 0x80;
//             //fprintf( stderr, "\n%x\n", i_value);
//             // i % 8 is required so that values above 8 are at transformed to the correct index.
//             int output_index = i % 8;
//             if ( output_index == 0 )
//                 output_index = 8;
                
//             i_value = i_value >> output_index;
//             // if ( (i/8) == 1 ) {
//             //     fprintf( stderr, "\n-------%s\n", "HERE");
//             //     fprintf( stderr, "\n-------%x\n", i_value);
//             // }
//             //fprintf( stderr, "\n%x\n", i_value);
//             //fprintf( stderr, "\n%x\n", i_value);
//             output[ i / 8 ] = output[ i / 8 ] | i_value;
            
//         }
//         else {
//             //i_value = 0xFF00;
//             //fprintf( stderr, "\n%x\n", i_value);
//             // fprintf( stderr, "\n%x\n", i_value);
//             if ( i >= 1 ) {
//                 signed char s_value = 0x80;

//                 int output_index = i % 8;
//                 if ( output_index == 0 )
//                     output_index = 8;

//                 s_value = s_value >> ( output_index - 1 );
//                 output[ i / 8 ] = output[ i / 8 ] & s_value;
//                 // fprintf( stderr, "\n-------%x\n", s_value);
//             }
//             else {
//                 i_value = 0x00;

//                 int output_index = i % 8;
//                 if ( output_index == 0 )
//                     output_index = 8;

//                 i_value = i_value >> ( output_index );
//                 output[ i / 8 ] = output[ i / 8 ] & i_value;
//                 // fprintf( stderr, "\n-------%x\n", i_value);
//             }
//             // fprintf( stderr, "\n-------%x\n", i_value);
//             // output[ i / 8 ] = output[ i / 8 ] & i_value;
//         }
//         // fprintf( stderr, "\n%x\n", i / 8);
//         fprintf( stderr, "\n%x\n", output[ 0 ]);
//         fprintf( stderr, "%x\n", output[ 1 ]);
//         //fprintf( stderr, "\n%x\n", i_value);

//         // Copy the input array value to the output array.

//         output[ i ] = output[ i ] | i_value;

//         i_index = 0x80;
//     }

//     // Pad the ending of output with 0s if n is not a multiple of 8.
//     if ( ( n % 8 ) != 0 ) {
//         int n_remainder = n % 8;

//         // If n is 9, the remainer will be 1. This must be changed to 7 as 7 0s must be added, not 1.
//         //n_remainder = ( 8 - n_remainder );
//         fprintf( stderr, "\n HERE \n");
//         for ( int i = n_remainder + 1; i <= 8; i++ ) {
//             i_value = 0xFEFF >> i;

//             output[ a_index ] = output[ a_index ] & i_value;
//         }
//     }

    // fprintf( stderr, "\n%x\n", output[ 0 ] );
    // fprintf( stderr, "%x\n", output[ 1 ] );
}

void generateSubkeys( byte K[ ROUND_COUNT ][ SUBKEY_BYTES ], byte const key[ BLOCK_BYTES ] ) {
    // Create the left and right permutation from the given key.
    byte left_perm[ SUBKEY_HALF_BYTES ];
    permute( left_perm, key, leftSubkeyPerm, SUBKEY_HALF_BITS );

    byte right_perm[ SUBKEY_HALF_BYTES ];
    permute( right_perm, key, rightSubkeyPerm, SUBKEY_HALF_BITS );
    // for ( int b = 0; b < 4; b++ ) {
    //         fprintf( stderr, "\n%x\n", right_perm[ b ] );
    // }

    // Left rotate the bits in the key based on the subkey shift schedule.
    
    // will loop through the shift schedule (starting at 1).
        // At each index. I will getbit() those bits (1 or 2) and put them on the other side
        // with each one of these (c1 & d1, c2 & d2...) they should be combined into a 56 bit key and permuted into a 48 bit key.
        // these 48 bit keys should be stored in K as the sub keys.

    int c_bit_one = 0;
    int c_bit_two = 0;
    int c_bit_three = 0;
    int c_bit_four = 0;

    int d_bit_one = 0;
    int d_bit_two = 0;
    int d_bit_three = 0;
    int d_bit_four = 0;

    for ( int i = 1; i < ROUND_COUNT; i++ ) {
        // If the shift schedule is one, get one bit.
        if ( subkeyShiftSchedule[ i ] == 1 ) {
            // Get the first bit from each byte.
            c_bit_one = getBit( left_perm, 1 );
            c_bit_two = getBit( left_perm, 9 );
            c_bit_three = getBit( left_perm, 17 );
            c_bit_four = getBit( left_perm, 25 );

            // fprintf( stderr, "\n%d", c_bit_one );
            // fprintf( stderr, "\n%d", c_bit_two );
            // fprintf( stderr, "\n%d", c_bit_three );
            // fprintf( stderr, "\n%d", c_bit_four );

            d_bit_one = getBit( right_perm, 1 );
            d_bit_two = getBit( right_perm, 9 );
            d_bit_three = getBit( right_perm, 17 );
            d_bit_four = getBit( right_perm, 25 );

            // fprintf( stderr, "\n%d", d_bit_one );
            // fprintf( stderr, "\n%d", d_bit_two );
            // fprintf( stderr, "\n%d", d_bit_three );
            // fprintf( stderr, "\n%d", d_bit_four );

            // Shift each byte to the left by one.
            left_perm[ 0 ] = left_perm[ 0 ] << 1;
            left_perm[ 1 ] = left_perm[ 1 ] << 1;
            left_perm[ 2 ] = left_perm[ 2 ] << 1;
            left_perm[ 3 ] = left_perm[ 3 ] << 1;

            right_perm[ 0 ] = right_perm[ 0 ] << 1;
            right_perm[ 1 ] = right_perm[ 1 ] << 1;
            right_perm[ 2 ] = right_perm[ 2 ] << 1;
            right_perm[ 3 ] = right_perm[ 3 ] << 1;

            // Put each bit in the correct place.
            putBit( left_perm, 8, c_bit_two );
            putBit( left_perm, 16, c_bit_three );
            putBit( left_perm, 24, c_bit_four );
            putBit( left_perm, 28, c_bit_one );

            putBit( right_perm, 8, d_bit_two );
            putBit( right_perm, 16, d_bit_three );
            putBit( right_perm, 24, d_bit_four );
            putBit( right_perm, 28, d_bit_one );

            // fprintf( stderr, "\nONE");
            // // fprintf( stderr, "\n%x\n", left_perm );
            // // fprintf( stderr, "%x\n", right_perm );
            // fprintf( stderr, "\n%x\n", left_perm[ 0 ] );
            // fprintf( stderr, "\n%x\n", right_perm[ 0 ] );
        }
        // If the shift schedule is two, get two bits.
        else {
            for ( int j = 0; j < 2; j++ ) {
                c_bit_one = getBit( left_perm, 1 );
                c_bit_two = getBit( left_perm, 9 );
                c_bit_three = getBit( left_perm, 17 );
                c_bit_four = getBit( left_perm, 25 );

                d_bit_one = getBit( right_perm, 1 );
                d_bit_two = getBit( right_perm, 9 );
                d_bit_three = getBit( right_perm, 17 );
                d_bit_four = getBit( right_perm, 25 );

                // Shift each byte to the left by one.
                left_perm[ 0 ] = left_perm[ 0 ] << 1;
                left_perm[ 1 ] = left_perm[ 1 ] << 1;
                left_perm[ 2 ] = left_perm[ 2 ] << 1;
                left_perm[ 3 ] = left_perm[ 3 ] << 1;

                right_perm[ 0 ] = right_perm[ 0 ] << 1;
                right_perm[ 1 ] = right_perm[ 1 ] << 1;
                right_perm[ 2 ] = right_perm[ 2 ] << 1;
                right_perm[ 3 ] = right_perm[ 3 ] << 1;

                // Put each bit in the correct place.
                putBit( left_perm, 8, c_bit_two );
                putBit( left_perm, 16, c_bit_three );
                putBit( left_perm, 24, c_bit_four );
                putBit( left_perm, 28, c_bit_one );

                putBit( right_perm, 8, d_bit_two );
                putBit( right_perm, 16, d_bit_three );
                putBit( right_perm, 24, d_bit_four );
                putBit( right_perm, 28, d_bit_one );

                // fprintf( stderr, "\nTWO");
                // // fprintf( stderr, "\n%x\n", left_perm );
                // // fprintf( stderr, "%x\n", right_perm );
                // fprintf( stderr, "\n%x\n", left_perm[ 0 ] );
                // fprintf( stderr, "\n%x\n", right_perm[ 0 ] );
            }
        }

        // Combine the subkeys into one 56 bit key.
        // When combining, we need to get rid of the last four bits in each key to make them truly 28 bits
        byte left_right_key[ 7 ];

        // Copy bytes from the left perm.
        for ( int b = 0; b < 4; b++ ) {
            if ( b < 3 ) {
                left_right_key[ b ] = left_perm[ b ];
            }
            // On the last by, get the last four bits from the left perm and the first four bits from the second perm.
            else {
                byte last_byte = 0x00;

                last_byte = last_byte | left_perm[ b ];
                //last_byte = last_byte << 4;
                last_byte = last_byte | ( right_perm[ 0 ] >> 4 );

                left_right_key[ b ] = last_byte;
            }
        }

        // Copy the bytes from the right perm. Each byte is a combination of the last and first four bits from sequential bytes.
        for ( int b = 4; b < 7; b++ ) {
            byte last_byte = 0x00;

            last_byte = last_byte | ( right_perm[ b - 4 ] << 4 );
            //last_byte = last_byte << 4;
            last_byte = last_byte | ( right_perm[ b - 3 ] >> 4 );

            left_right_key[ b ] = last_byte;
        }

        // Permute the 56 bit key combination into a 48 bit key.
        byte output_key[ 6 ];
        permute( output_key, left_right_key, subkeyPerm, SUBKEY_BITS );

        // Add the permuted 48 bit key to the K array.
        for ( int b = 0; b < 6; b++ ) {
            K[ i ][ b ] = output_key[ b ];
        }
        // fprintf( stderr, "\n%x\n", output_key );
    }
}

void sBox( byte output[ 1 ], byte const input[ SUBKEY_BYTES ], int idx ) {
    // B is one of the 6 byte/48 bit elements of K.
    // First get the index to start in input at using idx * 6 + 1 formula
        // get the 6 bits
        // Get the row index from first and last bit
        // get column index
        // the result from the s table is transformed to binary and put in output
        // What table of sbox is used depends on idx ( at the end)
    // ___-_-_--_-_--____---____--___- Note that idx counts from zero for B1, one for B2

    int index = idx * 6;
    byte b1[ 6 ] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    for ( int i = 1; i < 7; i++ ) {
        index += 1;

        // Which byte of input the index is in.
        int input_byte = ( index - 1 ) / 8;

        // Which bit of the input byte the index is.
        int input_bit = index % 8;
        if ( input_bit == 0 )
            input_bit = 8;

        // Move the bit to the lowest order bit.
        unsigned char input_value = ( input[ input_byte ] >> ( 8 - input_bit ) ) & 0x01;

        // Set the corresponding array value to the value of the bit.
        b1[ i - 1 ] = input_value;
    }

    // Determine the row and column values
    int row = ( (b1[ 0 ] << 1) | b1[ 5 ] );
    int col = ( (b1[ 1 ] << 3 ) | 
                (b1[ 2 ] << 2 ) | 
                (b1[ 3 ] << 1 ) | 
                (b1[ 4 ]) );

    // Determine what index of sbox to use.
    index = idx * 6 + 1;
    int sbox_index = (index - 1) / 6;

    int sbox_output = sBoxTable[ sbox_index ][ row ][ col ];

    // Convert the sbox integer to binary
    byte output_byte = 0x00;

    int test_bit = 8;

    while ( test_bit != 0 ) {
        if ( sbox_output / test_bit != 0 ) {
            if ( test_bit == 8 ) {
                output_byte = output_byte | 0x80;
            }
            else if ( test_bit == 4 ) {
                output_byte = output_byte | 0x40;
            }
            else if ( test_bit == 2 ) {
                output_byte = output_byte | 0x20;
            }
            else if ( test_bit == 1 ) {
                output_byte = output_byte | 0x10;
            }
            sbox_output -= test_bit;

        }

        test_bit /= 2;
    }

    // Set the output byte to the transformed sbox value.
    output[ 0 ] = output_byte;
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

    for ( int i = 0; i < 8; i++ ) {
        // sBox() each six bits of B into four bit values.
        sBox( sbox_bits, B, i);

        int val = 0;
        for ( int b = 1; b <= 4; b++ ) {
            // Obtain each bit from the four bit value.
            val = getBit( sbox_bits, b );
            // Put that bit value at the correct index in the result.
            putBit( sbox_result, ( 4 * i + b ), val );
        }
    }

    // Permute the result into the correct result.
    permute( result, sbox_result, fFunctionPerm, 32 );
}

void encryptBlock( DESBlock *block, byte const K[ ROUND_COUNT ][ SUBKEY_BYTES ] ) {
    //take DESBlock and permute into two separate 32 bit arrays
        // Each time, combine R with K(i) using the fFunction()
        // Combine with previous L value using exclusive or
        // After all of this, combine the L and R blocks together

    byte L[ BLOCK_HALF_BYTES ];
    byte R[ BLOCK_HALF_BYTES ];
    byte l_next[ BLOCK_HALF_BYTES ];
    byte r_next[ BLOCK_HALF_BYTES ];
    // for ( int i = 0; i < 6; i++ ) {
    //     fprintf( stderr, "\n%x\n", K[ 1 ][ i ]);
    //     fprintf( stderr, "\n%x\n", block->data[ i ]);
    //     fprintf( stderr, "\n------------\n" );
    // }

    permute( L, block->data, leftInitialPerm, 32 );
    permute( R, block->data, rightInitialPerm, 32 );
    // fprintf( stderr, "\n%x\n", R[ 2 ] );

    for ( int i = 1; i <= 16; i++ ) {
        // Perform the fFunction() or R using the corresponding K value.
        fFunction( r_next, R, K[ i ] );

        // Copy R into l_next.
        for ( int b = 0; b < 4; b++ )
            l_next[ b ] = R[ b ];

        // Set R equal to the exclusive or value of r_next and L.
        for ( int b = 0; b < 4; b++ )
            R[ b ] = r_next[ b ] ^ L[ b ];

        // Set L to the previous value of R, which is l_next.
        for ( int b = 0; b < 4; b++ )
            L[ b ] = l_next[ b ];
    }

    // Combine R and L.
    byte final[ BLOCK_BYTES ];
    for ( int i = 0; i < 8; i++ ) {
        if ( i < 4 )
            final[ i ] = R[ i ];
        else
            final[ i ] = L[ i - 4 ];
    }

    // Permute this final value back into the result block.
    permute( block->data, final, finalPerm, BLOCK_BITS );
}

void decryptBlock( DESBlock *block, byte const K[ ROUND_COUNT ][ SUBKEY_BYTES ] ) {
    byte L[ BLOCK_HALF_BYTES ];
    byte R[ BLOCK_HALF_BYTES ];
    byte l_next[ BLOCK_HALF_BYTES ];
    byte r_next[ BLOCK_HALF_BYTES ];

    permute( L, block->data, leftInitialPerm, 32 );
    permute( R, block->data, rightInitialPerm, 32 );

    for ( int i = 16; i >= 1; i-- ) {
        // Perform the fFunction() or R using the corresponding K value.
        fFunction( r_next, R, K[ i ] );

        // Copy R into l_next.
        for ( int b = 0; b < 4; b++ )
            l_next[ b ] = R[ b ];

        // Set R equal to the exclusive or value of r_next and L.
        for ( int b = 0; b < 4; b++ )
            R[ b ] = r_next[ b ] ^ L[ b ];

        // Set L to the previous value of R, which is l_next.
        for ( int b = 0; b < 4; b++ )
            L[ b ] = l_next[ b ];
    }

    byte final[ BLOCK_BYTES ];
    for ( int i = 0; i < 8; i++ ) {
        if ( i < 4 )
            final[ i ] = R[ i ];
        else
            final[ i ] = L[ i - 4 ];
    }

    // Permute this final value back into the result block.
    permute( block->data, final, finalPerm, BLOCK_BITS );
}
