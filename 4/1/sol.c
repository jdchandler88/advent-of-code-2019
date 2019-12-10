#include "sol.h"
#include "utils.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

bool testPasswordRangeString(const char* range) {
    //format is int-int
    int numRange;
    const char** ranges = parseString(range, "-", &numRange);
    if (numRange != 2) {
        printf("hmm. supposed to have 2 input integers separated by a '-'. but we got %s", range);
    }

    int beginRange = atoi(ranges[0]);
    int endRange = atoi(ranges[1]);
    int count = 0;
    for (int i=beginRange; i<=endRange; i++) {
        char password[6];
        sprintf(password, "%d", i);
        if (testPassword(beginRange, endRange, password)) {
            count++;
            printf("found a possible password: %i\n", i);
        }
    }
    printf("There are %i possible passwords.\n", count);
}

/**
 * 
    It is a six-digit number.
    The value is within the range given in your puzzle input.
    Two adjacent digits are the same (like 22 in 122345).
    Going from left to right, the digits never decrease; they only ever increase or stay the same (like 111123 or 135679).

**/
bool testPassword(int beginRange, int endRange, char* password) {
    //check length
    int len = strlen(password);
    if (len != 6) {
        return false;
    }

    //digit check
    /**
     * Using strtol becuase error checking is more robust.
     * 
     * From strtol's doc:
     * -The implementation may also set errno to EINVAL in case no conversion
       was performed (no digits seen, and 0 returned).
       -If there were no digits at all,
       strtol() stores the original value of nptr in *endptr (and returns
       0).
    **/
    char* passwordEnd;
    int result = (int)strtol(password, &passwordEnd, 10);
    if (errno == EINVAL || errno == ERANGE || (result == 0 && password == passwordEnd)) {  //if base bad (shouldn't happen, number out of range, or no parsing occured)
        return false;
    }

    //range check
    if (result < beginRange || result > endRange) {
        return false;
    }

    //adjacent and never decreasing check
    //let's slide across the password
    bool adjacent = false;   //if it's found, then let's store the result
    for (int i=0; i<len-1; i++) {
        char current = password[i];
        char next = password[i+1];
        //adjacent
        adjacent |= (current == next);

        //dever decreasing (can short-circuit if rule breaks)
        if (next < current) {
            return false;
        }
    }

    //this is the last check
    return adjacent;
}