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
  // Does not include new line character __-_-___--_--___---____--_____--____----____----____-
  char *return_str = v->vptr;
  printf( "%s", return_str );
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
  if ( other->print != printString )
    return false;

  char *v_string = v->vptr;
  char *o_string = other->vptr;

  if ( strcmp( v_string, o_string ) == 0 ) {
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
  int size = 1;
  int str_len = strlen( v->vptr ) - 1;
  // printf( "\n%d\n", str_len );
  char *str_value = v->vptr;

  while ( size < str_len ) {
    // printf( "\n%d\n", str_value[ size ] );
    hash_value += ( str_value[ size++ ] );
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

// CITE as being based on input.c __-----___---____--_____---____---__-_--____---__-
int parseString( Value *v, char const *str )
{
  // int len = 0;
  // int num_read = 0;
  // int capacity = 5;

  // char *text_string = ( char * ) malloc( capacity * sizeof( char ) );
  // char cur_char = ' ';

  // int scan_i = 0;
  // scan_i = sscanf( str, "%c", &cur_char );
  // // num_read++;

  // bool first_quote = false;
  // bool second_quote = false;

  // while ( true ) {
  //   //printf( "\n%c\n", cur_char );
  //   if ( scan_i == EOF ) {
  //     break;
  //   }

  //   if ( len + 1 >= capacity ) {
  //     //printf( "\nHERE4\n");
  //     capacity *= 2;
  //     text_string = ( char * ) realloc( text_string, capacity * sizeof( char ) );
  //   }

  //   if ( cur_char == '"' && first_quote ) {
  //     //printf( "\nHERE3\n");
  //     text_string[ len ] = cur_char;
  //     text_string[ len + 1 ] = '\0';
  //     len++;

  //     break;
  //   }

  //   if ( cur_char == '"' && !first_quote ) {
  //     first_quote = true;
  //     //printf( "\nHERE1\n");
  //   }

  //   if ( first_quote && !second_quote ) {
  //     //printf( "\nHERE2\n");
  //     text_string[ len ] = cur_char;
  //     text_string[ len + 1 ] = '\0';
  //     len++;
  //   }
  //   // If the first quote has not been included, increment the number read.
  //   else {
  //     num_read++;
  //   }

  //   scan_i = 0;
  //   // printf( "\n%c\n", cur_char );
  //   // printf( "%d\n", num_read );
  //   scan_i = sscanf( str + len + num_read, "%c", &cur_char );
  // }

  // // Fill in all the fields of v for an integer type of value.
  // v->print = printString;
  // v->move = moveString;
  // v->equals = equalsString;
  // v->hash = hashString;
  // v->empty = emptyString;
  // v->vptr = text_string;

  // // Return how much of str we parsed.
  // return len + num_read;

  // _______________________________________________________

  int len = 0;
  int num_read = 0;
  int capacity = 5;

  char *text_string = ( char * ) malloc( capacity * sizeof( char ) );
  char cur_char = ' ';

  int scan_i = 0;
  scan_i = sscanf( str, "%c", &cur_char );
  // num_read++;

  bool first_quote = false;
  bool second_quote = false;

  while ( true ) {
    //printf( "\n%c\n", cur_char );
    if ( scan_i == EOF ) {
      break;
    }

    if ( len + 1 >= capacity ) {
      //printf( "\nHERE4\n");
      capacity *= 2;
      text_string = ( char * ) realloc( text_string, capacity * sizeof( char ) );
    }

    // Extra credit forward slash.
    if ( cur_char == '\\' ) {
      // Get the character after the new line.
      num_read++;
      sscanf( str + len + num_read, "%c", &cur_char );

      //fprintf( stderr, "\n%c\n", cur_char );

      if ( cur_char == 'n') {
        text_string[ len ] = '\n';
        text_string[ len + 1 ] = '\0';
        len++;
      }
      else if ( cur_char == 't' ) {
        text_string[ len ] = '\t';
        text_string[ len + 1 ] = '\0';
        len++;
      }
      else if ( cur_char == '\\' ) {
        text_string[ len ] = '\\';
        text_string[ len + 1 ] = '\0';
        len++;
      }
      else if ( cur_char == '\"' ) {
        text_string[ len ] = cur_char;
        text_string[ len + 1 ] = '\0';
        len++;
      }

      scan_i = 0;
      scan_i = sscanf( str + len + num_read, "%c", &cur_char );

      continue;
    }

    if ( cur_char == '"' && first_quote ) {
      //printf( "\nHERE3\n");
      text_string[ len ] = cur_char;
      text_string[ len + 1 ] = '\0';
      len++;

      break;
    }

    if ( cur_char == '"' && !first_quote ) {
      first_quote = true;
      //printf( "\nHERE1\n");
    }

    if ( first_quote && !second_quote ) {
      //printf( "\nHERE2\n");
      text_string[ len ] = cur_char;
      text_string[ len + 1 ] = '\0';
      len++;
    }
    // If the first quote has not been included, increment the number read.
    else {
      num_read++;
    }

    scan_i = 0;
    // printf( "\n%c\n", cur_char );
    // printf( "%d\n", num_read );
    scan_i = sscanf( str + len + num_read, "%c", &cur_char );
  }

  // Fill in all the fields of v for an integer type of value.
  v->print = printString;
  v->move = moveString;
  v->equals = equalsString;
  v->hash = hashString;
  v->empty = emptyString;
  v->vptr = text_string;

  // Return how much of str we parsed.
  return len + num_read;
}
