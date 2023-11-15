
// This component is responsible for reading input and writing output files a block at a time.

#include "io.h"
#include "DES.h"
#include <stdio.h>

void readBlock( FILE *fp, DESBlock *block ) {
    block->len = fread( block->data, 1, 8, fp );
}

void writeBlock( FILE *fp, DESBlock const *block ) {
    fwrite( block->data, 1, block->len, fp );
}
