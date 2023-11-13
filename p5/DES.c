/** 
    @file DES.c
    @author
    Implementation of the DES algorithm.
*/

// This is where the DES algorithms for encryption and decryption are implemented. 
// It’s broken up into several small functions for the various steps, so these can be tested and debugged independently

#include "DES.h"
#include "DESMagic.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//This function checks the given text key to make sure it’s not too long. 
// It copies the characters of this key from textKey to the key array and pads with zero bytes up to the length of a DES block.
void prepareKey( byte key[ BLOCK_BYTES ], char const *textKey ) {
    int key_len = strlen( textKey );

    if ( key_len > 8 ) {
        fprintf( stderr, "Key too long\n" );
        exit( EXIT_FAILURE );
    }

    for ( int i = 0; i < key_len; i++ ) {
        key[ i ] = textKey[ i ];
        // if ( key[ i ] == 0x61 ) {
        //     fprintf( stderr, "\n_-_--__--___\n");
        // }
        // else {
        //     fprintf( stderr, "\n NO \n");
        // }

        //sprintf( key[ i ], "%x", textKey[ i ] );
        //fprintf( stderr, "\n_-_--__--___\n");
        //fprintf( stderr, "%x", key[ i ] );
    }

    if ( key_len < 8 ) {
        while ( key_len < 8 ) {
            key[ key_len ] = 0;
            key_len++;
        }
    }

    // for ( int i = 0; i < key_len; i++ ) {
    //     fprintf( stderr, "%x\n", key[ i ]);
    // }
}

// This function returns zero or one based on the value of the bit at index idx 
// in the given array of bytes. For example, getbit( data, 1 ) should return 1 if the high-order 
// bit of the first byte of data is set. This function, and the next one will make it 
// very easy to perform the bit manipulation used by the DES algorithm
int getBit( byte const data[], int idx ) {
    // get length of data
    // create 

    // maybe determine what array index idx is in (3 would be in index 1)
    // create and binary/hex value based on idx
    // compare the array block and this

    int a_index = (idx - 1) / 8;
    //fprintf( stderr, "\n%x\n", data[ a_index ]);
    int norm_index = idx;
    if ( idx > 8 ) {
        norm_index = idx % 8;
    }

    int b_index = 8 - norm_index;
    //fprintf( stderr, "\n%d\n", b_index);
    // if ( ( data[ a_index ] & ( 0x01 << b_index ) ) != 0 )
    //     return 1;
    // else
    //     return 0;
    return ( data[ a_index ] & ( 0x01 << b_index ) ) != 0;
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
    unsigned char i_value = 0x00;

    for ( int i = 0; i < n; i++ ) {
        p_idx = 0;
        p_byte = 0;
        i_idx = 0;
        i_value = 0x00;
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

            output_value = output_value >> ( n - 8 );

            output[ 1 ] = output[ 1 ] & ( unsigned char ) output_value;
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

// void generateSubkeys( byte K[ ROUND_COUNT ][ SUBKEY_BYTES ], byte const key[ BLOCK_BYTES ] ) {

// }

// void sBox( byte output[ 1 ], byte const input[ SUBKEY_BYTES ], int idx ) {

// }

// void fFunction( byte result[ BLOCK_HALF_BYTES ], byte const R[ BLOCK_HALF_BYTES ], byte const K[ SUBKEY_BYTES ] ) {

// }

// void encryptBlock( DESBlock *block, byte const K[ ROUND_COUNT ][ SUBKEY_BYTES ] ) {

// }

// void decryptBlock( DESBlock *block, byte const K[ ROUND_COUNT ][ SUBKEY_BYTES ] ) {

// }
