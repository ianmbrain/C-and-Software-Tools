/**
 * @file util.c
 * @author Ian M Brain (imbrain)
 * This file provides utility functions that are used by other files in the program.
 * Skip digits returns the frist index of a non digit in a string.
 * Skip letters returns the first index of a non letter in a string.
 * Copy substring enables a string to be copied onto another string while specifying the starting point of copying.
  */

#include "util.h"

#include <ctype.h>

/**
 * Searches through the parameter array and finds the first index of a value that is not a digit. This index is then returned.
 * @param str Array to search for.
 * @param start Index to start searching from in the array.
 * @return Index of first value in the array that is not a digit.
*/
int skip_digits( char str[], int start )
{
  int return_index = start;

  while ( str[ return_index ] ) {
    if ( !isdigit( str[ return_index ] ) ) {
      return return_index;
    }

    return_index++;
  }

  return return_index;
}

/**
 * Searches through the parameter array and finds the first index of a value that is not a letter. This index is then returned.
 * @param str Array to search for.
 * @param start Index to start searching from in the array.
 * @return Index of first value in the array that is not a letter.
*/
int skip_letters( char str[], int start )
{
  int return_index = start;

  while ( str[ return_index ] ) {
    if ( !isalpha( str[ return_index ] ) ) {
      return return_index;
    }

    return_index++;
  }

  return return_index;
}

/**
 * Copies part of a source array to a destination array. Callers should ensure that the destination array is long enough for the copied portion.
 * The characters copied from the source array start at the sstart index and go up to but not including the send index.
 * These characters are copied to the destination array starting at the dstart value. A null terminator is added at the index after the copied value.
 * @param dest Array to copy to.
 * @param dstart Index in the destination array to place the copy at.
 * @param src Array to copy from.
 * @param sstart Index in source array to begin copying from.
 * @param send Index in source array to copy up to and where the null terminator should be placed in the destination array.
 * @return Index of the null terminator in the destincation array.
*/
int copy_substring( char dest[], int dstart, char src[], int sstart, int send )
{
  for ( int i = sstart; i < send; i++ ) {
    dest[ dstart ] = src[ i ];
    dstart++;
  }

  dest[ dstart ] = '\0';
  return send;
}