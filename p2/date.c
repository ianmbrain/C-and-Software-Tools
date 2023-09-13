/**
  */

#include "date.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
