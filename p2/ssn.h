/**
 * @file ssn.h
 * @author Ian M Brain (imbrain)
 * This file provides functionality to read in the individuals social security number and format it correctly.
 * The correct format for a social security number is ###-##-####.
 * These two functions are used in formatter.c in order to read and format user input.
 */

#include "util.h"
#include <stdbool.h>

/** String indicating no SSN */
#define SSN_NA "N/A"

/** Exit status for a missing or badly formatted ssn. */
#define SSN_ERROR 103

/** Number of digits in the first group in the SSN. */
#define START_DIGITS 3

/** Number of digits in the middle group in the SSN. */
#define MIDDLE_DIGITS 2

/** Number of digits in the last group in the SSN. */
#define END_DIGITS 4

/**
 * Read in the user inputted ssn to the ssn parameter variable from standard input.
 * Exit with 103 status if the ssn cannot be read or it is too long.
 * @param ssn ssn variable to store the user inputted ssn to.
*/
void read_ssn( char ssn[ FIELD_MAX + 1 ] );

/**
 * Updates the ssn to the format of three digits, dash, two digits, dash, final four digits.
 * Exit with 103 status if the ssn is not in one of three formats:
 * ###-##-####,
 * nine consecutive digits,
 * or N/A.
 * @param ssn ssn to update to the correct format. 
*/
void fix_ssn( char ssn[ FIELD_MAX + 1 ] );
