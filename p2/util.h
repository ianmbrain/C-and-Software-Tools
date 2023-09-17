/**
 */

/** Maximum length of any field in the input. */
#define FIELD_MAX 30

int skip_digits( char str[], int start );
int skip_letters( char str[], int start );
int copy_substring( char dest[], int dstart, char src[], int sstart, int send );
