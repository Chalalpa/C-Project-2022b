//
// Created by Moshe on 02/08/2022.
//

#include <string.h>
#include "consts.h"

struct Directive {
    char* name;
    int minOperandsNum;
    int maxOperandsNum;
};


static const struct Directive DIRECTIVES_TABLE[5] = {
        {DATA, 0, MAX_LINE_LEN},
        {STRING, 1, 1},
        {STRUCT, 2, 2},
        {ENTRY, 1, 1},
        {EXTERN, 1, 1},
};


struct Directive* getDirectiveByName(char* directiveName);