/**
 * @file date.h
 * @author Ian M Brain (imbrain)
 * This file provides functionality to read in the individuals phone number and format it correctly.
 * The correct format for a phone number is YYYY-MM-DD.
 * These two functions are used in formatter.c in order to read and format user input.
 */

#include "util.h"
#include <stdbool.h>

/**
 * Reads in the date from standard input and exits with 102 status if the name does not exist or is not 8 or 10 characters in length.
 * @param date Date variable to store the inputted date in.
*/
void read_date( char date[ FIELD_MAX + 1 ] );

/**
 * Transforms the date into the proper YYYY-MM-DD format.
 * Exit with 102 status if the date is not in one of the specified formats:
 * YYYY-DD-MM, DD/MM/YYYY, DD/MM/YY, DD-MM-YYYY, DD-MM-YY.
 * @param date Date variable to be transformed.
*/
void fix_date( char date[ FIELD_MAX + 1 ] );
