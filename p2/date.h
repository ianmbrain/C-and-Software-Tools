/**
 * @file date.h
 * @author Ian M Brain (imbrain)
 * This file provides functionality to read in the individuals phone number and format it correctly.
 * The correct format for a phone number is YYYY-MM-DD.
 * These two functions are used in formatter.c in order to read and format user input.
 */

#include "util.h"
#include <stdbool.h>

/** Exit status for a missing or badly formatted date. */
#define DATE_ERROR 102

/** Two low-order digits of the current year. */
#define CURRENT_YEAR "23"

/** Number of digits in the short (2 digit) year. */
#define SHORT_YEAR 2

/** Number of digits in the full (4 digit) year. */
#define FULL_YEAR 4

/** Number of digits in the month. */
#define MONTH_DIGITS 2

/** Number of digits in the day. */
#define DAY_DIGITS 2

/** Length of the date with short year. */
#define SHORT_YEAR_LENGTH 8

/** Length of the date with full year. */
#define FULL_YEAR_LENGTH 10

/** Index of the first separator for dates that start with a full year. */
#define FULL_FIRST_SEPARATOR 4

/** Index of the second separator for dates that start with a full year. */
#define FULL_SECOND_SEPARATOR 7

/** Index of the first separator for dates that do not start with a full year. */
#define SHORT_FIRST_SEPARATOR 2

/** Index of the second separator for dates that do not start with a full year. */
#define SHORT_SECOND_SEPARATOR 5

/** Index of the first digit in a four digit date. */
#define FIRST_YEAR_DIGIT 2

/** Index of the second digit in a four digit date. */
#define SECOND_YEAR_DIGIT 3

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
