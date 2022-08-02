//
// Created by Moshe on 02/08/2022.
//
#include "directives.h"

struct Directive* getDirectiveByName(char* directiveName) {
    int i;
    for (i=0; i<5; i++) {
        if (!strcmp(DIRECTIVES_TABLE[i].name, directiveName))
            return &DIRECTIVES_TABLE[i];
    }
    return 0;
}
