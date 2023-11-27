/**
    @file value.c
    @author Ian M Brain (imbrain)
    Implementation for the value component.
    Provides functionality for integer and string values.
    Each value type has a method to print, move, equal, hash, and empty the value.
    Each value type also has a method to parse the integer or string value from string input.
    This functionality is used in map to work with each value struct.
    This functionality is used in driver to parse integers and strings from values.
  */

#include "value.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/** Initial capacity of the string memory address. */
#define INIT_CAPACITY 5

/** Double the allowable memory capacity when reallocing. */
#define CAPACITY_RESIZE 2

//////////////////////////////////////////////////////////
// Integer implementation.

// print method for Integer.
// This method was provided by teaching staff for project 6.
static void printInteger( Value const *v )
{
  // Print the integer inside this value.
  printf( "%d", v->ival );
}

// move method for Integer.
// This method was provided by teaching staff for project 6.
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
// This method was provided by teaching staff for project 6.
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
// This method was provided by teaching staff for project 6.
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

// Print method for strings.
static void printString( Value const *v )
{
  char *return_str = v->vptr;
  printf( "%s", return_str );
}

// Move method for strings.
static void moveString( Value const *src, Value *dest )
{
  dest->vptr = src->vptr;

  dest->print = src->print;
  dest->move = src->move;
  dest->equals = src->equals;
  dest->hash = src->hash;
  dest->empty = src->empty;
}

// Equals method for strings.
static bool equalsString( Value const *v, Value const *other )
{
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
  // Hash value of the value string.
  unsigned int hash_value = 0;
  // Size of the hash value to ensure it is not longer than the string. Value is one to ensure the first quote is not inlcuded.
  int size = 1;
  // Length of the string. Minused by one to ensure the last quote is not included.
  int str_len = strlen( v->vptr ) - 1;
  // Value of the string.
  char *str_value = v->vptr;

  // For each character in the string, add the character to the hash value.
  while ( size < str_len ) {
    hash_value += ( str_value[ size++ ] );
    hash_value += hash_value << 10;
    hash_value ^= hash_value >> 6;
  }

  // Perform the hash operation on the string.
  hash_value += hash_value << 3;
  hash_value ^= hash_value >> 11;
  hash_value += hash_value << 15;

  // Return the hash value.
  return hash_value;
}

// Empty method for strings.
static void emptyString( Value *v )
{
  free( v->vptr );
}

// CITE as being based on input.c __-----___---____--_____---____---__-_--____---__-
int parseString( Value *v, char const *str )
{
  // Len of the string used to realloc.
  int len = 0;
  // Number of characters read not included in the string.
  int num_read = 0;
  // Capacity of the string used for realloc.
  int capacity = INIT_CAPACITY;

  // String containing parsed test.
  char *text_string = ( char * ) malloc( capacity * sizeof( char ) );
  // Current character of the parsed text.
  char cur_char = ' ';

  // Value to determine if end of file is reached.
  int scan_i = 0;
  scan_i = sscanf( str, "%c", &cur_char );

  // Boolean indicating if the first quote was reached.
  bool first_quote = false;
  // Boolean indicating if the last quote was reached.
  bool second_quote = false;

  while ( true ) {
    // Stop scanning when end of file is reached.
    if ( scan_i == EOF ) {
      break;
    }

    // Realloc the string memory when capacity is reached. Plus one ensures there is space for the null terminator.
    if ( len + 1 >= capacity ) {
      capacity *= CAPACITY_RESIZE;
      text_string = ( char * ) realloc( text_string, capacity * sizeof( char ) );
    }

    // Functionality to permit escapte sequences. 
    if ( cur_char == '\\' ) {
      // If there is a forward slash, get the character after the new line.
      num_read++;
      sscanf( str + len + num_read, "%c", &cur_char );

      // Add new line escape sequence if the next character is n.
      if ( cur_char == 'n') {
        text_string[ len ] = '\n';
        text_string[ len + 1 ] = '\0';
        len++;
      }
      // Add tab escape sequence if the next character is t.
      else if ( cur_char == 't' ) {
        text_string[ len ] = '\t';
        text_string[ len + 1 ] = '\0';
        len++;
      }
      // Add forward slash escape sequence if the next character is forward slash.
      else if ( cur_char == '\\' ) {
        text_string[ len ] = '\\';
        text_string[ len + 1 ] = '\0';
        len++;
      }
      // Add single quatation escape sequence if the next character is n.
      else if ( cur_char == '\"' ) {
        text_string[ len ] = cur_char;
        text_string[ len + 1 ] = '\0';
        len++;
      }

      // Continue onto the next character.
      scan_i = 0;
      scan_i = sscanf( str + len + num_read, "%c", &cur_char );

      continue;
    }

    // If the last quote is reached and first quote was already reached, stop reading.
    if ( cur_char == '"' && first_quote ) {
      text_string[ len ] = cur_char;
      text_string[ len + 1 ] = '\0';
      len++;

      break;
    }

    // If the first quote is reached, set the first quote boolean.
    if ( cur_char == '"' && !first_quote ) {
      first_quote = true;
    }

    // If the first quote has been reached but not the second quote, add the character to the string.
    if ( first_quote && !second_quote ) {
      text_string[ len ] = cur_char;
      text_string[ len + 1 ] = '\0';
      len++;
    }
    // If the first quote has not been included, increment the number read but don't add the character to the string.
    else {
      num_read++;
    }

    // Read the next character and repeat the loop
    scan_i = 0;
    scan_i = sscanf( str + len + num_read, "%c", &cur_char );
  }

  // Fill in all the fields of v for an integer type of value.
  v->print = printString;
  v->move = moveString;
  v->equals = equalsString;
  v->hash = hashString;
  v->empty = emptyString;
  v->vptr = text_string;

  // Return how much of the string was parsed.
  return len + num_read;
}
