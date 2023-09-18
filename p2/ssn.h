/**
 */

#include "util.h"
#include <stdbool.h>

/** String indicating no SSN */
#define SSN_NA "N/A"

void read_ssn( char ssn[ FIELD_MAX + 1 ] );
void fix_ssn( char ssn[ FIELD_MAX + 1 ] );
