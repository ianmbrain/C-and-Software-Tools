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

int copy_substring( char dest[], int dstart, char src[], int sstart, int send )
{
  for ( int i = sstart; i < send; i++ ) {
    dest[ dstart ] = src[ i ];
    dstart++;
  }

  dest[ dstart ] = '\0';
  return send;
}