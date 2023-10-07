/**
 * @file util.c
 * @author Ian M Brain (imbrain)
 * This file provides functionality to check if certain calculations of unsigned long integers overflow.
 * Functionality includes checking for overflow in the addition, subtraction, or multiplication of integers.
 * Used.... DO THIS -__________-----_______---_____----______
  */

#include "util.h"
#include <limits.h>

/**
 * Ensures that two unsigned long values can be added without overflowing.
 * Returns true if the addition does not overflow and otherwise false.
 * @param a first number in the addition.
 * @param b second number in the addition.
 * @return true if the addition of a and b do not overflow and otherwise false.
*/
bool checkAdd( unsigned long a, unsigned long b ) {
  if ( b > ULONG_MAX - a ) {
    return false;
  }

  return true;
}

/**
 * Ensures that two unsigned long integers can be subtracted without overflowing.
 * This is checked by ensuring that the difference in a and b is not less than 0.
 * Return false if the difference in a and b is less than 0 and true otherwise.
 * @param a first number in the subtraction.
 * @param b number to subtract from a.
 * @return true if the difference of a and b do not overflow and false otherwise.
*/
bool checkSub( unsigned long a, unsigned long b ) {
  if ( a - b < 0 ) {
    return false;
  }

  return true;
}

/**
 * Ensures that two unsigned long integers can be multiplied without overflowing.
 * This is checked by ensuring that b is not greater than ULONG_MAX divided by a.
 * Return false if the product of a and b overflows and true otherwise.
 * @param a first number in the multiplation.
 * @param b number to multiply by a.
 * @return true if the product of a and b do not overflow and false otherwise.
*/
bool checkMul( unsigned long a, unsigned long b ) {
  if ( b > ULONG_MAX / a ) {
    return false;
  }

  return true;
}
