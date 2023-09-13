/**
  */

#include "ssn.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/** Exit status for a missing or badly formatted ssn. */
#define SSN_ERROR 103

/** Number of digits in the first group in the SSN. */
#define START_DIGITS 3

/** Number of digits in the middle group in the SSN. */
#define MIDDLE_DIGITS 2

/** Number of digits in the last group in the SSN. */
#define END_DIGITS 4
