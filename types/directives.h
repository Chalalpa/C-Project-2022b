#ifndef _DIRECTIVES_
#define _DIRECTIVES_

#include <string.h>
#include "../helpers/consts.h"

/* Represents a Directive type (data, extern, struct...) */
struct Directive {
    char* name;
    int minOperandsNum;
    int maxOperandsNum;
};


/* A static const represents the supported directives and their configurations*/
static const struct Directive DIRECTIVES_TABLE[5] = {
        {DATA, 0, MAX_LINE_LEN},
        {STRING, 1, 1},
        {STRUCT, 2, 2},
        {ENTRY, 1, 1},
        {EXTERN, 1, 1},
};


/* @ Function: struct Directive* getDirectiveByName(char* directiveName);
   @ Arguments: char* directiveName
   directiveName - is the requested directive name
   @ Description: The function goes through the directives array, and finds the requested directive by its name.
   It returns is if it finds it, but if not, it returns 0.
*/
struct Directive* getDirectiveByName(char* directiveName);

#endif