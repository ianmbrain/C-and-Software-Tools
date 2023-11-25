/**
    @file value.c
    @author
    Implementation for the value component, with support for integer
    and (eventually) string values.
  */

#include "value.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//////////////////////////////////////////////////////////
// Integer implementation.

// print method for Integer.
static void printInteger( Value const *v )
{
  // Print the integer inside this value.
  printf( "%d", v->ival );
}

// move method for Integer.
static void moveInteger( Value const *src, Value *dest )
{
  // We just need to copy the value.
  dest->ival = src->ival;
  
  dest->print = src->print;
  dest->move = src->move;
  dest->equals = src->equals;
  dest->hash = src->hash;
  dest->empty = src->empty;
}

// equals method for Integer.
static bool equalsInteger( Value const *v, Value const *other )
{
  // Make sure the other object is also an Integer.
  // (i.e., it uses the same print funtion)
  if ( other->print != printInteger )
    return false;

  return v->ival == other->ival;
}

// hash method for Integer.
static unsigned int hashInteger( Value const *v )
{
  // This will overflow negative values to large positive ones.
  return v->ival;
}

// Free memory used inside this integer Value.
static void emptyInteger( Value *v )
{
  // An int vaue doesn't need any additional memory.
}

int parseInteger( Value *v, char const *str )
{
  // Try to parse an integer from str.
  int val, len;
  if ( sscanf( str, "%d%n", &val, &len ) != 1 )
    return 0;

  // Fill in all the fields of v for an integer type of value.
  v->print = printInteger;
  v->move = moveInteger;
  v->equals = equalsInteger;
  v->hash = hashInteger;
  v->empty = emptyInteger;
  v->ival = val;

  // Return how much of str we parsed.
  return len;
}

//////////////////////////////////////////////////////////
// String implementation.

static void printString( Value const *v )
{
  printf( "%s", v->vptr );
}

static void moveString( Value const *src, Value *dest )
{
  dest->vptr = src->vptr;

  dest->print = src->print;
  dest->move = src->move;
  dest->equals = src->equals;
  dest->hash = src->hash;
  dest->empty = src->empty;
}

static bool equalsString( Value const *v, Value const *other )
{
  // Make sure the other object is also an Integer.
  // (i.e., it uses the same print funtion)
  if ( other->print != printInteger )
    return false;

  if ( strcmp( v, other ) == 0 ) {
    return true;
  }
  else {
    return false;
  }
}

/**
 * This code is based on the Jenkins Hash Function found in this wikipedia article: https://en.wikipedia.org/wiki/Jenkins_hash_function
*/
static unsigned int hashString( Value const *v )
{
  unsigned int hash_value = 0;
  int size = 0;
  int str_len = strlen( v->vptr );
  char *str_value = v->vptr;

  while ( size < str_len ) {
    hash_value += ( str_value[ size++ ] - '0' );
    hash_value += hash_value << 10;
    hash_value ^= hash_value >> 6;
  }

  hash_value += hash_value << 3;
  hash_value ^= hash_value >> 11;
  hash_value += hash_value << 15;
  return hash_value;
}

static void emptyString( Value *v )
{
  free( v->vptr );
}

int parseString( Value *v, char const *str )
{
  int len;
  char str_value[20] = "";
  if ( sscanf( str, "%s%n", str_value, &len ) != 1 )
    return 0;

  char *str = ( char * ) malloc( strlen( str_value ) +1 );
  strcpy( str, str_value );

  // Fill in all the fields of v for an integer type of value.
  v->print = printString;
  v->move = moveString;
  v->equals = equalsString;
  v->hash = hashString;
  v->empty = emptyString;
  v->vptr = str;

  // Return how much of str we parsed.
  return len;
}
