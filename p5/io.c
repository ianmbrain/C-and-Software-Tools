/**
 * @file io.c
 * @author Ian M Brain (imbrain)
 * This file provides functionality to read and write eight byte blocks of data from a file to a provided DESBlock.
 * These functions are used in encrypt.c and decrypt.c to read data from input files and write data to ouput files.
*/

#include "io.h"
#include "DES.h"
#include <stdio.h>

void readBlock( FILE *fp, DESBlock *block ) {
    block->len = fread( block->data, 1, 8, fp );
}

void writeBlock( FILE *fp, DESBlock const *block ) {
    fwrite( block->data, 1, block->len, fp );
}
