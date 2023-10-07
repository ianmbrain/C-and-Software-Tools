#!/bin/bash
# This is a shell script to check your program(s) on test cases.
# It usese the same language you normally use to type commands in
# a terminal window.  You can write whole programs in shell.

# Assume we've succeeded until we see otherwise.
FAIL=0

# Print an error message and set the fail flag.
fail() {
    echo "**** $1"
    FAIL=1
}

# Check the exit status.  The actual exit status (ASTATUS) should match
# the expected status (ESTATUS)
checkStatus() {
  ESTATUS="$1"
  ASTATUS="$2"

  # Make sure the program exited with the right exit status.
  if [ "$ASTATUS" -ne "$ESTATUS" ]; then
      fail "FAILED - incorrect program exit status. (expected $ESTATUS,  Got: $ASTATUS)"
      return 1
  fi

  return 0
}

# Check the contents of an a file.  If the expected file (EFILE)
# exists, then the actual file (AFILE) should exist and it should match.
checkFile() {
  NAME="$1"
  EFILE="$2"
  AFILE="$3"

  # Make sure we're really expecting this file.
  if [ ! -f "$EFILE" ]; then
      return 0
  fi

  # Make sure the output matches the expected output.
  echo "   diff $EFILE $AFILE"
  diff -q "$EFILE" "$AFILE" >/dev/null 2>&1
  if [ $? -ne 0 ]; then
      fail "FAILED - $NAME ($AFILE) doesn't match expected ($EFILE)"
      return 1
  fi

  return 0
}

# Same as checkFile, but if the expected file (EFILE) doesn't exist, the
# actual file (AFILE) should be empty.
checkFileOrEmpty() {
  NAME="$1"
  EFILE="$2"
  AFILE="$3"
  
  # if the expected output file doesn't exist, the actual file should be
  # empty.
  if [ ! -f "$EFILE" ]; then
      if [ -s "$AFILE" ]; then
	  fail "FAILED - $NAME ($AFILE) should be empty."
	  return 1
      fi
      return 0
  fi

  # Make sure the output matches the expected output.
  echo "   diff $EFILE $AFILE"
  diff -q "$EFILE" "$AFILE" >/dev/null 2>&1
  if [ $? -ne 0 ]; then
      fail "FAILED - $NAME ($AFILE) doesn't match expected ($EFILE)"
      return 1
  fi

  return 0
}

# Same as checkFile, but if the expected file (EFILE) doesn't exist, the
# actual file (AFILE) should not exist.
checkFileOrDNE() {
  NAME="$1"
  EFILE="$2"
  AFILE="$3"
  
  # if the expected output file doesn't exist, the actual file should not exist
  if [ ! -f "$EFILE" ]; then
      if [ -f "$AFILE" ]; then
	  fail "FAILED - $NAME ($AFILE) should not exist."
	  return 1
      fi
      return 0
  fi

  # Make sure the output matches the expected output.
  echo "   diff $EFILE $AFILE"
  diff -q "$EFILE" "$AFILE" >/dev/null 2>&1
  if [ $? -ne 0 ]; then
      fail "FAILED - $NAME ($AFILE) doesn't match expected ($EFILE)"
      return 1
  fi

  return 0
}

# The given file should exist but should be empty.
checkEmpty() {
  NAME="$1"
  AFILE="$2"
  
  if [ -s "$AFILE" ]; then
      fail "FAILED - $NAME ($AFILE) should be empty."
      return 1
  fi

  return 0
}

# Test the ident program.
runTest() {
    ACCT=$1
    VERSION=$2
    TESTNO=$3
    ESTATUS=$4

    echo "Test $TESTNO"
    if [ "$VERSION" != "x" ]; then
	rm -f "$ACCT-$((VERSION + 1)).txt"
    fi

    echo "   ./trader "$ACCT-$VERSION.txt" tlist-$TESTNO.txt ${extraArgs[@]} > output.txt 2> stderr.txt"
    ./trader "$ACCT-$VERSION.txt" tlist-$TESTNO.txt ${extraArgs[@]} > output.txt 2> stderr.txt
    ASTATUS=$?

    if ! checkStatus "$ESTATUS" "$ASTATUS"
	    ! checkEmpty "Program output" "output.txt" ||
	    ! checkFileOrDNE "Output account file" "expected-$TESTNO.txt" "$ACCT-$((VERSION + 1)).txt" ||
	    ! checkFileOrEmpty "Stderr output" "expected-message-$TESTNO.txt" "stderr.txt"
    then
	FAIL=1
	return 1
    fi

    echo "Test $TESTNO PASS"
    return 0
}

# make a fresh copy of the target programs
make clean
make

if [ $? -ne 0 ]; then
  fail "Make exited unsuccessfully"
fi

# Run test cases
if [ -x trader ]; then
    extraArgs=()
    runTest alice 1 01 0
    
    extraArgs=()
    runTest alice 1 02 0
    
    extraArgs=()
    runTest alice 1 03 0
    
    extraArgs=()
    runTest alice 1 04 0
    
    extraArgs=()
    runTest aliceAndBob 5 05 0
    
    extraArgs=()
    runTest aliceAndBob 5 06 0
    
    extraArgs=()
    runTest tenAccounts 25 07 0
    
    extraArgs=()
    runTest tenAccounts 25 08 0
    
    extraArgs=()
    runTest example 7 09 0
    
    extraArgs=()
    runTest extremes 1 10 0
    
    extraArgs=()
    runTest tenSpacing 430 11 0
    
    extraArgs=()
    runTest longName 1 12 0
    
    extraArgs=()
    runTest aliceAndBob 5 13 0
    
    extraArgs=()
    runTest 100_accounts 1 14 0
    
    extraArgs=()
    runTest 10000_accounts 1 15 0
    
    extraArgs=()
    runTest fileWithAReallyLongName 57 16 0
    
    extraArgs=()
    runTest badBalance 1 17 1
    
    extraArgs=()
    runTest badBalance 3 18 1
    
    extraArgs=()
    runTest badBalance 5 19 1
    
    extraArgs=()
    runTest extremes 1 20 1
    
    extraArgs=()
    runTest extremes 1 21 1
    
    extraArgs=()
    runTest extremes 1 22 1
    
    extraArgs=()
    runTest tooLongName 1 23 1
    
    extraArgs=()
    runTest badVersion x 25 1
    
    extraArgs=()
    runTest fileWithAReallyLongName 99 26 1
    
    extraArgs=()
    runTest nonexistentFile 1 27 1
    
    extraArgs=(extra-argument)
    runTest alice 1 28 1
    
else
    fail "Since your program didn't compile, we couldn't test it"
fi

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  echo "TESTS SUCCESSFUL"
  exit 0
fi
