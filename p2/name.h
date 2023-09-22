/**
 * @file name.h
 * @author Ian M Brain (imbrain)
 * This file provides functionality to read in the individuals name and format it correctly.
 * The correct format for a name is lastname, firstname.
 * These two functions are used in formatter.c in order to read and format user input.
 */

#include "util.h"
#include <stdbool.h>

/** Exit status for a missing or badly formatted name. */
#define NAME_ERROR 101

/** Two characters that represent the space and comma inbetween the first and last name. */
#define COMMA_SPACE 2

/** One character that represents the space between the first and last name. */
#define SPACE_INDEX 1

/**
 * Reads in the name from standard input and stores it in the parameter string
 * Returns false if end of file is reached and there is not text.
 * Exits with 101 status if the name is longer than 30 characters or does not exist.
 * @param name Variable to store the name within.
 * @return False if end of file is reached and there is no name field. Exit with 101 status error if the name is empty or too long. Return true otherwise. 
*/
bool read_name( char name[ FIELD_MAX + 1 ] );

/**
 * Transforms the parameter name into the correct format.
 * Capitalizes the first letter of the first and last name.
 * Reverse the order of the names and add a comma between them if there is not a comma separating the names.
 * Exits with 101 status if the name contains non characters besides a comma and a space or contains spaces within names.
 * @param name Name to be fixed.
*/
void fix_name( char name[ FIELD_MAX + 1 ] );
