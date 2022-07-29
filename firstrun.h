//
// Created by Moshe on 29/07/2022.
//

#include "consts.h"
#include <ctype.h>
#include <unistd.h>
#include "common_utils.h"


struct Symbol {
    char* name;  // The macro name to be used later in the source code
    char* data;  // The macro content itself to deploy later in the code
    struct Macro* next;  // A pointer to the next Macro struct object
};


int firstRun(char* file_name);

