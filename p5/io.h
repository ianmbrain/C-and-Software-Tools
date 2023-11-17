
#include "DES.h"
#include <stdio.h>

/**
 * Reads up to eight bytes of data from the file and stores them within the specified block.
 * The file must have been opened with the read binary access type.
 * @param fp file to read bytes from opened with the read binary access type.
 * @param block block to store eight byte pieces of the file within.
*/
void readBlock( FILE *fp, DESBlock *block );

/**
 * Takes up to eight bytes of data from the block and writes them to the specified file.
 * The file must have been opened with the write binary access type.
 * @param fp file to write bytes to opened with the write binary access type.
 * @param block block to read eight byte pieces of the file from.
*/
void writeBlock( FILE *fp, DESBlock const *block );
